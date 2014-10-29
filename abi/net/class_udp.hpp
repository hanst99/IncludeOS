#ifndef CLASS_UDP_HPP
#define CLASS_UDP_HPP

#include <net/class_ip4.hpp>
#include <map>

namespace net {

  class UDP{
  public:
  
    /** UDP port number */
    typedef uint16_t port;
  
    /** A protocol buffer temporary type. Later we might encapsulate.*/
    typedef uint8_t* pbuf;
  
    /** UDP header */
    struct udp_header {
      port sport;
      port dport;
      uint16_t length;
      uint16_t checksum;
    };
  
    /** Full UDP Header with all sub-headers */
    struct full_header{
      Ethernet::header eth_hdr;
      IP4::ip_header ip_hdr;
      udp_header udp_hdr;
    }__attribute__((packed));

    /** Input from network layer */
    int bottom(uint8_t* data, int len);
  
    /** Delegate type for listening to UDP ports. */
    typedef delegate<int(uint8_t* data,int len)> listener;
  
    /** Delegate type for downstream transmissions */
    typedef IP4::transmitter network_out;
  
    /** Delegate output to network layer */
    inline void set_network_out(network_out del)
    { _network_layer_out = del; }
  
    /** Listen to a port. 
      
        @note Any previous listener will be evicted (it's all your service) */
    void listen(uint16_t port, listener s);

    /** Send UDP datagram from source ip/port to destination ip/port. 
      
        @param sip Source IP address - must be bound to a local interface.
        @param sport source port; replies might come back here
        @param dip Destination IP
        @param dport Destination port   */
    int transmit(IP4::addr sip,UDP::port sport,
                 IP4::addr dip,UDP::port dport,
                 uint8_t* data, int len);
  
    UDP();
  
  private: 
  
    network_out _network_layer_out;
    std::map<uint16_t, listener> ports;
  
  };

}

#endif