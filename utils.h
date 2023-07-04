#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void ip_dot(uint32_t);
void ipv4_address_set_dot(uint32_t);
uint16_t calculate_checksum(const void *, size_t);

#endif //UTILS_H