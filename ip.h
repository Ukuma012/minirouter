#ifndef IP_H
#define IP_H

#define IPV4_PROTOCOL_LENGTH 4
#define ICMP_PROTOCOL 1
#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
#define IPV4_ADDRESS(a, b, c, d) (a * 0x1000000u + b * 0x10000 + c * 0x100 + d)
#define TTL_VALUE 64

struct ipv4_device
{
    uint32_t ipv4_address;
    uint32_t subnet_mask;
    uint32_t broadcast_addr;
};

struct ipv4_header
{
    uint8_t version_ihl;
    uint8_t type_of_service;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t TTL;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t source_ipv4_addr;
    uint32_t destination_ipv4_addr;
} __attribute__((__packed__));

struct net_device;
struct mbuf;
struct routing_trie_node;

void ipv4_dump(unsigned char *);
void ipv4_address_routing_set(struct routing_trie_node *, struct net_device *, uint32_t, uint32_t);
void ipv4_input(struct net_device *, unsigned char *, ssize_t);
void ipv4_output(struct mbuf *, uint32_t, uint32_t, uint8_t);
uint32_t ipv4_get_network_addr(uint32_t, uint32_t);

#endif // IP_H