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
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "tcp_client.h"

#define INVALID_SOCKET (-1)


namespace gr {
  namespace difi {

      tcp_client::tcp_client(std::string ip_addr, uint32_t port):
        d_socket(INVALID_SOCKET)
      {
        memset(&d_servaddr, 0, sizeof(d_servaddr));
        d_servaddr.sin_family = AF_INET;
        d_servaddr.sin_port = htons(port);
        d_servaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        create_socket();
      }

      tcp_client::~tcp_client()
      {
          shutdown(d_socket, SHUT_RDWR);
          close(d_socket);
      }

      void tcp_client::create_socket()
      {
        if ((d_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
          std::cerr << "Could not make TCP socket, socket may be in use." << std::endl;
          throw std::runtime_error("Could not make TCP socket");
        }

        int enable;
        if (setsockopt(d_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)) < 0)
        {
          std::cerr << "setsockopt(SO_REUSEADDR) failed." << std::endl;
          throw std::runtime_error("setsockopt(SO_REUSEADDR) failed.");
        }
      }

      bool tcp_client::connect()
      {
          int res = ::connect(d_socket, (struct sockaddr*)&d_servaddr, sizeof(d_servaddr));
          return (res == 0)? true:false;
      }

      bool tcp_client::is_connected()
      {
        // check connection has been established by getting the peer information
        socklen_t addr_len = sizeof(d_servaddr);
        if(getpeername(d_socket, (struct sockaddr*)&d_servaddr, &addr_len) < 0)
        {
          shutdown(d_socket, SHUT_RDWR);
          close(d_socket);
          create_socket();
          return false;
        }

        // monitor the connection for errors
        struct pollfd pollfd;

        pollfd.fd = d_socket;
        pollfd.events = POLLIN | POLLOUT | POLLERR;

        int rc = poll(&pollfd, 1, (int)100);

        if(rc < 0)
        {
          std::cerr << "Failed to poll socket." << std::endl;
          return false;
        }
        else if(rc > 0) // zero means the poller timed out and there are no events in the socket
        {
          if(pollfd.revents != POLLIN)
          {
            int error = 0;
            socklen_t errlen = sizeof(error);
            getsockopt(d_socket, SOL_SOCKET, SO_ERROR, (void *)&error, &errlen);

            if(pollfd.revents != POLLOUT or error != 0)
            {
              //std::cerr << "An unexpected poller revent: " << pollfd.revents << " result error:" << error << std::endl;

              shutdown(d_socket, SHUT_RDWR);
              close(d_socket);
              create_socket();
              return false;
            }
          }
        }

        return true;
      }

      int tcp_client::send(int8_t* buf, int len)
      {
        int num_bytes_sent = ::send(d_socket, buf, len, 0);

        if(num_bytes_sent != len)
        {
          std::cerr << "Send failed to send msg on socket correctly" << std::endl;
         throw std::runtime_error("Send failed to send msg on socket correctly");
        }

        return num_bytes_sent;
      }
  }
}
