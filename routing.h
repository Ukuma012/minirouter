#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>

#define IPV4_LENGTH 32

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

void routing_binary_tree_add(struct routing_trie_node *, uint32_t, uint32_t, struct routing_entry *);
struct routing_trie_node * routing_binary_search(struct routing_trie_node *, uint32_t, uint32_t);

#endif // ROUTING_H