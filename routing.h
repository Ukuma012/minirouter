#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>

struct routing_trie_node {
    int is_leaf;
    char dev_name[32];
    uint32_t transfer_ip_addr;
    bool is_directly_connected;
    bool is_next_hop;
    struct routing_trie_node *node_zero;
    struct routing_trie_node *node_one;
};

#endif // ROUTING_H