#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void ipv4_ntoh_dot(uint32_t);
uint16_t calculate_checksum(const void *, size_t);

#endif //UTILS_H