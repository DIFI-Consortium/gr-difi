// -*- c++ -*-
// Copyright (c) Microsoft Corporation and Welkin Sciences, LLC.
// Licensed under the GNU General Public License v3.0 or later.
// See License.txt in the project root for license information.

#ifndef INCLUDED_DIFI_SOURCE_CPP_IMPL_H
#define INCLUDED_DIFI_SOURCE_CPP_IMPL_H

#include <deque>
#include <algorithm>
#include <complex>
#include <iostream>
#include <deque>
#include <iterator>
#include <pmt/pmt.h>

#include <difi/difi_common.h>
#include <difi/difi_source_cpp.h>

namespace gr {
namespace difi {

class tcp_server;
class udp_socket;
template <class T>
class difi_source_cpp_impl : public difi_source_cpp<T>
{
    double parse_vita_fixed_double(u_int64_t bits)
    {
        return (double)(bits) / (1<<20);
    }
    double parse_vita_fixed_double(int64_t bits)
    {
        return (double)(bits) / (1<<20);
    }
    float parse_vita_fixed_float(int16_t bits)
    {
        return (float)(bits) / (1<<7);
    }
    struct header_data {
        u_int16_t pkt_n;
        u_int8_t type;
        u_int32_t header;
        u_int32_t stream_num;
    };

    struct context_packet {
        u_int64_t class_id;
        u_int32_t full; //seconds
        u_int64_t frac; //picoseconds
        u_int32_t cif;
        u_int32_t ref_point;
        double bw; //Hertz
        double if_ref_freq; //Hertz
        double rf_ref_freq; //Hertz
        double if_band_offset; //Hertz
        float ref_lvl; //dBm
        float rf_gain, if_gain; //dB
        double samp_rate; //Hertz
        int64_t t_adj; //femtoseconds
        u_int32_t t_cal; //seconds
        u_int32_t state_indicators;
        u_int64_t payload_format;
    };


private:
    template <typename  M>
    static M unpack_16(int8_t *start)
    {
      int16_t re;
      memcpy(&re, start, 2);
      int16_t imag;
      memcpy(&imag, start + 2, 2);
      return M(re, imag);
    }

    template <typename  M>
    static M unpack_8(int8_t *start)
    {
      int8_t re;
      memcpy(&re, start, 1);
      int8_t imag;
      memcpy(&imag, start + 1, 1);
      return M(re, imag);
    }


    typedef enum
    {
        throw_exe = 0,
        ignore = 1,
        warnings_forward = 2,
        warnings_no_forward = 3
    }context_behavior;

    T (*d_unpacker)(int8_t *);
    void parse_header(header_data& data);
    pmt::pmt_t make_pkt_n_dict(int pkt_n, int size_gotten);
    void unpack_context(context_packet& context);
    void unpack_context_alt(context_packet& context);
    pmt::pmt_t make_context_dict(header_data& header, int size_gotten);
    int buffer_and_send(T* out, int noutput_items);

    bool d_send;
    u_int32_t d_last_full;
    u_int64_t d_last_frac;
    int32_t d_static_bits;
    u_int32_t d_unpack_idx_size;
    int d_behavior;
    int d_stream_number;
    long d_last_pkt_n;
    pmt::pmt_t d_context;
    std::vector<int8_t> d_packet_buffer;
    std::deque<char> d_deque;
    tcp_server* p_tcpserver;
    udp_socket* p_udpsocket;

public:
    difi_source_cpp_impl(std::string ip_addr,
                    uint32_t port,
                    uint8_t socket_type,
                    int stream_number,
                    int bit_depth,
                    int context_pkt_behavior);
    ~difi_source_cpp_impl();
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
    static const uint VITA_PKT_MOD = 16;
    static const uint MS_DATA_VITA_HEADER = 0x18;
};

} // namespace difi
} // namespace gr

#endif /* INCLUDED_DIFI_SOURCE_CPP_IMPL_H */
