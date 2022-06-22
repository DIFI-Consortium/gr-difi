// -*- c++ -*-
// Copyright (c) Microsoft Corporation.
// Licensed under the GNU General Public License v3.0 or later.
// See License.txt in the project root for license information.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "udp_socket.h"


namespace gr {
namespace difi {

    udp_socket::udp_socket(std::string ipaddr, uint32_t port, bool isServer, uint32_t sock_buffer_size)
    {

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        memset(&d_servaddr, 0, sizeof(d_servaddr));
        d_servaddr.sin_family = AF_INET;
        d_servaddr.sin_port = htons(port);

        if(isServer)
            d_servaddr.sin_addr.s_addr = INADDR_ANY;
        else
            d_servaddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());

        if ((d_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        {
            std::cerr << "Could not make UDP socket, socket may be in use." << std::endl;
            throw std::runtime_error("Could not make UDP socket");
        }

        if(setsockopt(d_socket, SOL_SOCKET, SO_RCVBUF, &sock_buffer_size, sizeof(sock_buffer_size)) < 0)
        {
            std::cerr << "Could not set socket size" << std::endl;
            throw std::runtime_error("Could not set socket size");
        }

        if (setsockopt(d_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
        {
            std::cerr << "Could not set timeout on socket" << std::endl;
            throw std::runtime_error("Could not set timeout on socket");
        }

        if(isServer)
        {
            if (bind(d_socket, (const struct sockaddr *)&d_servaddr,
                    sizeof(d_servaddr)) < 0)
            {
                std::cerr << "Could not connect to port, port may be in use" << std::endl;
                throw std::runtime_error("Could not connect to port");
            }
        }
    }

    udp_socket::~udp_socket()
    {
        close(d_socket);
    }

    int udp_socket::read(int8_t* buf, int bufsize)
    {
        socklen_t len = sizeof(d_servaddr);
        return recvfrom(d_socket, buf, bufsize,
                                    MSG_WAITALL, (struct sockaddr *)&d_servaddr, &len);
    }

    int udp_socket::send(int8_t* buf, int sz)
    {
        int num_bytes_sent = sendto(d_socket, buf, sz, 0, (const struct sockaddr *) &d_servaddr, sizeof(d_servaddr));
        if(num_bytes_sent != sz)
        {
            std::cerr << "Send failed to send msg on socket correctly" << std::endl;
            throw std::runtime_error("Send failed to send msg on socket correctly");
        }

        return num_bytes_sent;
    }

  } /* namespace difi */
} /* namespace gr */
