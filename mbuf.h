#ifndef MBUF_H
#define MBUF_H

#include <stdint.h>

struct mbuf {
    struct mbuf* prev;
    struct mbuf* next;
    uint32_t len;
    uint8_t buffer[];
};

struct mbuf *create(uint32_t);

#endif // MBUF_H