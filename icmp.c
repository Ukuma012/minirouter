#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
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

void icmp_input(uint32_t soruce_ip_addr, uint32_t destination_ip_addr, unsigned char *buffer, ssize_t len) {
    struct icmp_header *icmp_header;
    icmp_header = (struct icmp_header *)buffer;

    switch(icmp_header->type) {
        case ICMP_ECHO_REPLY:
            printf("%s\n", "ICMP ECHO REPLY arrives");
            return;
            
        case ICMP_ECHO_REQUEST:
            printf("%s\n", "ICMP ECHO REQUEST arrives"); 
            return;
    }
    return;
}