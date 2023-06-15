#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "arp.h"

struct arp_header {
   uint16_t hardware_type;
   uint16_t protocol_type;
   uint8_t hardware_len;
   uint8_t protocol_len;
   uint16_t operation_code;
   uint8_t source_mac_addr[6];
   uint32_t source_protocol_addr;
   uint8_t target_mac_addr[6];
   uint32_t target_protocol_addr;
} __attribute__ ((__packed__));

void arp_dump(unsigned char *buffer) {
    struct arp_header* arp_header;
    arp_header = (struct arp_header *)buffer;

    printf("%04X\n", ntohs(arp_header->hardware_type));

    return;
}