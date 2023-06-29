#ifndef MBUF_H
#define MBUF_H

#include <stdint.h>

struct mbuf {
    struct mbuf* prev;
    struct mbuf* next;
    uint32_t len;
    uint8_t buffer[];
};

void mbuf_init(struct mbuf *);
struct mbuf *mbuf_create(uint32_t);
void mbuf_memfree(struct mbuf *);

#endif // MBUF_H