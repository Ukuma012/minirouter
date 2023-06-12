#include "ethernet.h"
#include <stdio.h>
#include <stdint.h>

struct ethernet_header {
    uint8_t dest_mac_addr[6];
    uint8_t source_mac_addr[6];
    uint16_t ethertype;
} __attribute__((__packed__));