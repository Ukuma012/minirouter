#include <stdint.h>
#include <stdio.h>

void ip_dot(uint32_t ip) {
    uint8_t a = ip & 0x000000ff;
    uint8_t b = ip >> 8 & 0x000000ff;
    uint8_t c = ip >> 16 & 0x000000ff;
    uint8_t d = ip >> 24 & 0x000000ff;
    printf("%d.%d.%d.%d\n", a, b, c, d);
}

void ipv4_address_set_dot(uint32_t ip) {
    uint8_t d = ip >> 24 & 0x000000ff;
    uint8_t c = ip >> 16 & 0x000000ff;
    uint8_t b = ip >> 8 & 0x000000ff;
    uint8_t a = ip & 0x000000ff;
    printf("%d.%d.%d.%d\n", d, c, b, a);
}

uint16_t calculate_checksum(const void *data, size_t len) {
    const uint16_t *buffer = (const uint16_t *)data;
    uint32_t sum = 0;

    while(len > 1) {
        sum += *buffer++;
        len -= 2;
    }

    if(len > 0) {
        sum += *(const uint8_t *)buffer;
    }

    while(sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (uint16_t)(~sum);
}