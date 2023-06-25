#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "ip.h"
#include "net.h"
#include "utils.h"

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

void ipv4_dump(unsigned char *buffer)
{
    struct ipv4_header *ipv4_header;
    ipv4_header = (struct ipv4_header *)buffer;

    switch (ipv4_header->protocol)
    {
    case TCP_PROTOCOL:
        printf("%s\n", "TCP");
        return;
    case UDP_PROTOCOL:
        printf("%s\n", "UDP");
        return;
    case ICMP_PROTOCOL:
        printf("%s\n", "ICMP");
        return;
    default:
        printf("%d %s\n", ipv4_header->protocol, "Unknown IP protocol");
        break;
    }
    return;
}

void ipv4_address_set(struct net_device *dev, uint32_t ipv4_address, uint32_t subnet_mask)
{
    if (dev == NULL)
    {
        fprintf(stderr, "ipv4 address set failed\n");
        exit(1);
    }

    dev->ip_dev = (struct ipv4_device *)calloc(1, sizeof(struct ipv4_device));
    dev->ip_dev->ipv4_address = ipv4_address;
    dev->ip_dev->subnet_mask = subnet_mask;
    dev->ip_dev->broadcast_addr = (ipv4_address & subnet_mask) | (~subnet_mask);

    //    ipv4_address_set_dot(ipv4_address);
    //    ipv4_address_set_dot(subnet_mask);
    //    ipv4_address_set_dot(dev->ip_dev->broadcast_addr);

    printf("%s ip address set\n", dev->name);

    return;
}

void ipv4_input(struct net_device *input_dev, unsigned char *buffer, ssize_t len)
{
    struct ipv4_header *ipv4_header;
    ipv4_header = (struct ipv4_header *)buffer;

    for(struct net_device *dev = dev_base; dev; dev = dev->next) {
        if(dev->ip_dev->ipv4_address == ntohl(ipv4_header->destination_ipv4_addr)) {
            printf("%s\n", "Ah! these packets are for me!");
        }
    }

    switch (ipv4_header->protocol)
    {
    case TCP_PROTOCOL:
        printf("%s\n", "TCP");
        return;
    case UDP_PROTOCOL:
        printf("%s\n", "UDP");
        return;
    case ICMP_PROTOCOL:
        printf("%s\n", "ICMP");
        return;
    default:
        printf("%d %s\n", ipv4_header->protocol, "Unknown IP protocol");
        break;
    }
    return;
}