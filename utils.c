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