// -*- c++ -*-
// Copyright (c) Microsoft Corporation.
// Licensed under the GNU General Public License v3.0 or later.
// See License.txt in the project root for license information.

#ifndef INCLUDED_UDP_SOCKET_H
#define INCLUDED_UDP_SOCKET_H

#include <netinet/in.h>
#include <string>

namespace gr {
namespace difi {

class udp_socket
{
    public:

        // 64 MB request: SO_RCVBUF is clamped to net.core.rmem_max anyway, so
        // asking big simply takes whatever headroom the host allows (a few ms
        // of stall tolerance per MB at high sample rates).
        udp_socket(std::string ip_addr, uint32_t port, bool isServer, uint32_t socket_buffer_size=64000000);
        ~udp_socket();

        int read(int8_t* buf, int len);
        int send(int8_t* buf, int len);

    private:
        int d_socket;
        struct sockaddr_in d_servaddr;
};

} // namespace difi
} // namespace gr

#endif /* INCLUDED_UDP_SOCKET_H */