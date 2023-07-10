#include "routing.h"
#include <stdlib.h>
#include <stdio.h>

void routing_binary_tree_add(struct routing_trie_node *root, uint32_t ipv4_addr, uint32_t len, struct routing_entry *data) {
    struct routing_trie_node *current = root;

    for(int i = 1; i <= len; ++i) {
        if((ipv4_addr >> (IPV4_LENGTH - i)) & 0x01) {
            if(current->node_one == NULL) {
                struct routing_trie_node *tmp;
                if((tmp = malloc(sizeof(struct routing_trie_node))) < 0) {
                    fprintf(stderr, "malloc failed in routing table");
                    exit(1);
                }
                current->node_one = tmp;
                current->node_one->data = NULL;
                current->node_one->node_one = NULL;
                current->node_one->node_zero = NULL;
            }
            current = current->node_one;
        } else {
            if(current->node_zero == NULL) {
                struct routing_trie_node *tmp;
                if((tmp = malloc(sizeof(struct routing_trie_node))) < 0) {
                    fprintf(stderr, "malloc failed in routing table");
                    exit(1);
                }
                current->node_zero = tmp;
                current->node_zero->data = NULL;
                current->node_zero->node_one = NULL;
                current->node_zero->node_zero = NULL;
            }
            current = current->node_zero;
        }
    }

    current->data = data;
    return;
}

struct routing_trie_node * routing_binary_search(struct routing_trie_node *root, uint32_t target_ipv4_addr, uint32_t len) {
    struct routing_trie_node *current = root;

    for(int i = 1; i <= len; ++i) {
        if((target_ipv4_addr >> (IPV4_LENGTH - i)) & 0x01) {
            if(current->node_one == NULL) {
                printf("%s\n", "No such routing entry");
                return NULL;
            }
            current = current->node_one;
        } else {
            if(current->node_zero == NULL) {
                printf("%s\n", "No such routing entry");
                return NULL;
            }
            current = current->node_zero;
        }
    }

    return current;
}