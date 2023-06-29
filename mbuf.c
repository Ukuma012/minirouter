#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mbuf.h"

void mbuf_init(struct mbuf *mbuf) {
    mbuf->next = NULL;
    mbuf->prev = NULL;
    mbuf->len = 0;
}

struct mbuf *mbuf_create(uint32_t len) {
    struct mbuf *mbuf;
    if((mbuf = calloc(1, sizeof(struct mbuf) + len)) == NULL) {
        fprintf(stderr, "calloc failed\n");
        exit(1);
    }
   mbuf_init(mbuf);
   mbuf->len = len;
   return mbuf; 
}

void mbuf_memfree(struct mbuf *mbuf) {
    struct mbuf *current = mbuf;
    while(current->next != NULL) {
        current = current->next;
    }
    struct mbuf *tail = current;
    struct mbuf *tmp;
    while(tail != NULL) {
        tmp = tail;
        tail = tmp->prev;
        free(tmp);
    }
}