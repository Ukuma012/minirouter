#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>
#include "net.h"

enum routing_type {
    directly_connected,
    network
};

struct routing_entry {
    enum routing_type type;
    union{
        struct net_device *dev;
        uint32_t next_hop;
    };
};

struct routing_trie_node {
    struct routing_entry *data;
    struct routing_trie_node *node_zero;
    struct routing_trie_node *node_one;
};

#endif // ROUTING_H