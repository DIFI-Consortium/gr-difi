// -*- c++ -*-
// Copyright (c) Microsoft Corporation.
// Licensed under the GNU General Public License v3.0 or later.
// See License.txt in the project root for license information.

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "tcp_server.h"

#define INVALID_SOCKET (-1)


namespace gr {
  namespace difi {

      tcp_server::tcp_server(std::string ip_addr, uint32_t port):
        d_listener(INVALID_SOCKET),
        d_client(INVALID_SOCKET)
      {
        memset(&d_servaddr, 0, sizeof(d_servaddr));
        d_servaddr.sin_family = AF_INET;
        d_servaddr.sin_port = htons(port);
        d_servaddr.sin_addr.s_addr = INADDR_ANY;

        if ((d_listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
          std::cerr << "Could not make TCP socket, socket may be in use." << std::endl;
          throw std::runtime_error("Could not make TCP socket");
        }

        int enable;
        if (setsockopt(d_listener, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)) < 0)
        {
          std::cerr << "setsockopt(SO_REUSEADDR) failed." << std::endl;
          throw std::runtime_error("setsockopt(SO_REUSEADDR) failed.");
        }

        enable = 1;
        if(ioctl(d_listener, FIONBIO, (char *)&enable) < 0)
        {
          std::cerr << "ioctl(FIONBIO) failed." << std::endl;
          throw std::runtime_error("ioctl(FIONBIO) failed.");
        }

        if (bind(d_listener, (const struct sockaddr *)&d_servaddr,
                sizeof(d_servaddr)) < 0)
        {
          std::cerr << "Could not bind port, port may be in use." << std::endl;
          throw std::runtime_error("Could not bind port, port may be in use.");
        }

        if(listen(d_listener, 1) < 0)
        {
          std::cerr << "Error while listening to incoming connections." << std::endl;
          throw std::runtime_error("Error while listening to incoming connections.");
        }
      }

      tcp_server::~tcp_server()
      {
        if(d_client != INVALID_SOCKET)
        {
          shutdown(d_client, SHUT_RDWR);
          close(d_client);
        }

        if(d_listener != INVALID_SOCKET)
        {
          shutdown(d_listener, SHUT_RDWR);
          close(d_listener);
        }
      }

      bool tcp_server::is_client_connected()
      {
        if(d_client != INVALID_SOCKET)
          return true;

        check_for_incoming_conn();

        return d_client != INVALID_SOCKET;
      }

      int tcp_server::read(int8_t* buf, int len)
      {
        int total_bytes_read = 0;
        int remaining_bytes = len;
        while(total_bytes_read < len)
        {
          if(is_data_available())
          {
            int bytes_read = recv(d_client, buf+total_bytes_read, remaining_bytes, MSG_WAITALL);

            if(bytes_read <= 0)
            {
                reset_client_conn();
                return -1;
            }
            total_bytes_read += bytes_read;
            remaining_bytes -= bytes_read;
          }
        }
        return total_bytes_read;
      }

      void tcp_server::check_for_incoming_conn(int timeout_in_ms)
      {
        struct pollfd pollfd;

        pollfd.fd = d_listener;
        pollfd.events = POLLIN | POLLERR;

        int rc = poll(&pollfd, 1, (int)timeout_in_ms);

        if(rc < 0)
        {
          std::cerr << "Failed to poll listener." << std::endl;
        }
        else if(rc > 0) // zero means the poller timed out and there are no events in the socket
        {
          if(pollfd.revents == POLLIN)
          {
            socklen_t addrlen = sizeof(d_servaddr);
            d_client = accept(d_listener, (struct sockaddr*)&d_servaddr, &addrlen);
            std::cout << "Client connection has been accepted!" << std::endl;
          }
          else
          {
            std::cerr << "An unexpected poller revent result." << std::endl;
          }
        }
      }

      bool tcp_server::is_data_available(int timeout_in_ms)
      {
        struct pollfd pollfd;

        pollfd.fd = d_client;
        pollfd.events = POLLIN | POLLERR;

        int rc = poll(&pollfd, 1, (int)timeout_in_ms);

        if(rc < 0)
        {
          std::cerr << "Failed to poll client." << std::endl;
        }
        else if(rc > 0)
        {
          if(pollfd.revents == POLLIN)
          {
            return true;
          }
          else
          {
            std::cerr << "An unexpected poller revent result." << std::endl;
          }
        }
        return false;
      }

      void tcp_server::reset_client_conn()
      {
        std::wcerr << "Client connection has been closed." << std::endl;
        shutdown(d_client, SHUT_RDWR);
        close(d_client);
        d_client = INVALID_SOCKET;
      }

      int tcp_server::num_bytes_available()
      {
        if(d_client == INVALID_SOCKET)
          return -1;

        int available = 0;
        if(ioctl(d_client, FIONREAD, &available) < 0)
        {
          std::cerr << "ioctl(client, FIONREAD) failed." << std::endl;
          throw std::runtime_error("ioctl(client, FIONREAD) failed.");
        }
        return available;
      }
  }
}