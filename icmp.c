#include <stdio.h>
#include <stdint.h>
#include "icmp.h"

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
} __attribute__((packed));

void icmp_dump(unsigned char *buffer) {
    struct icmp_header *icmp_header;
    icmp_header = (struct icmp_header *)buffer;

    printf("icmp type: %u\n", icmp_header->type);
    printf("icmp code: %u\n", icmp_header->code);
    printf("icmp checksum: %u\n", icmp_header->checksum);

    return;
}