#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mbuf.h"

struct mbuf *create(uint32_t len) {
    struct mbuf *buf;
    if((buf = calloc(1, sizeof(struct mbuf) + len)) == NULL) {
        fprintf(stderr, "calloc failed\n");
        exit(1);
    }
   buf->len = len;
   return buf; 
}