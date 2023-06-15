#include "ip.h"
#include <stdio.h>
#include <stdint.h>

struct ipv4_header {
   uint8_t version_ihl;
   uint8_t type_of_service;
   uint16_t total_length;
   uint16_t identification;
   uint16_t flags_fragment_offset;
   uint8_t TTL;
   uint8_t protocol;
   uint16_t header_checksum;
   uint32_t source_ipv4_addr;
   uint32_t destination_ipv4_addr; 
} __attribute__ ((__packed__));

void ipv4_input(unsigned char *buffer) {
    struct ipv4_header *ipv4_header;
    ipv4_header = (struct ipv4_header *)buffer;

    switch(ipv4_header->protocol) {
        case TCP_PROTOCOL:
            printf("%s\n", "TCP");
            return;
        case UDP_PROTOCOL:
            printf("%s\n", "UDP");
            return;
        default:
            printf("%s\n", "Unknown IP protocol");
            break;
    }
    return;
}