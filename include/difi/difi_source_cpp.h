// -*- c++ -*- //
// Copyright (c) Microsoft Corporation.
// Licensed under the GNU General Public License v3.0 or later.
// See License.txt in the project root for license information.
//

#ifndef INCLUDED_DIFI_SOURCE_CPP_H
#define INCLUDED_DIFI_SOURCE_CPP_H

#include <difi/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace difi {

    template <class T>
    class DIFI_API difi_source_cpp : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<difi_source_cpp<T>> sptr;

      static sptr make(std::string ip_addr, uint32_t port, uint8_t socket_type, int stream_number, int bit_depth, int context_pkt_behavior);

    };
    typedef difi_source_cpp<gr_complex> difi_source_cpp_fc32;
    typedef difi_source_cpp<std::complex<char>> difi_source_cpp_sc8;

  } // namespace difi
} // namespace gr

#endif /* INCLUDED_DIFI_SOURCE_CPP_H */

