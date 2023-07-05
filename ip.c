#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "ip.h"
#include "arp.h"
#include "ethernet.h"
#include "net.h"
#include "utils.h"
#include "mbuf.h"
#include "icmp.h"

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
        icmp_dump(buffer + sizeof(struct ipv4_header));
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

    printf("%s ip address set\n", dev->name);

    return;
}

void ipv4_input(struct net_device *input_dev, unsigned char *buffer, ssize_t len)
{
    struct ipv4_header *ipv4_header;
    ipv4_header = (struct ipv4_header *)buffer;

    for (struct net_device *dev = dev_base; dev; dev = dev->next)
    {
        if (dev->ip_dev->ipv4_address == ntohl(ipv4_header->destination_ipv4_addr))
        {
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
                icmp_input(ntohl(ipv4_header->source_ipv4_addr), ntohl(ipv4_header->destination_ipv4_addr), buffer + sizeof(struct ipv4_header), len - sizeof(struct ipv4_header));
                return;
            default:
                printf("%d %s\n", ipv4_header->protocol, "Unknown IP protocol");
                break;
            }
        }
    }
    return;
}

void ipv4_output(struct mbuf *payload, uint32_t source_addr, uint32_t dest_addr, uint8_t protocol_number)
{
    uint16_t total_len = 0;
    struct mbuf *current = payload;
    while (current != NULL)
    {
        total_len += current->len;
        current = current->next;
    }

    struct mbuf *ipv4_mbuf = mbuf_create(sizeof(struct ipv4_header));
    payload->prev = ipv4_mbuf;
    ipv4_mbuf->next = payload;

    struct ipv4_header *ipv4_header;
    ipv4_header = (struct ipv4_header *)ipv4_mbuf->buffer;

    ipv4_header->version_ihl = 0x45;
    ipv4_header->type_of_service = 0;
    ipv4_header->total_length = htons(sizeof(struct ipv4_header) + total_len);
    ipv4_header->identification = 0;
    ipv4_header->flags_fragment_offset = 0;
    ipv4_header->TTL = TTL_VALUE;
    ipv4_header->protocol = protocol_number;
    ipv4_header->header_checksum = 0;
    ipv4_header->source_ipv4_addr = htonl(source_addr);
    ipv4_header->destination_ipv4_addr = htonl(dest_addr);
    ipv4_header->header_checksum = calculate_checksum(ipv4_header, sizeof(struct ipv4_header));

    for(struct net_device *dev = dev_base; dev; dev = dev->next) {
        if(dev->ip_dev == NULL || dev->ip_dev->ipv4_address == IPV4_ADDRESS(0,0,0,0)) {
            continue;
        }

        // if same network
        if((dev->ip_dev->ipv4_address & dev->ip_dev->subnet_mask) == (ntohl(ipv4_header->destination_ipv4_addr) & dev->ip_dev->subnet_mask)) {
            struct arp_entry *arp_entry;
            arp_entry = arp_cash_search(dest_addr);
            if(arp_entry == NULL) {
                arp_request(dev, dest_addr);
                mbuf_memfree(payload);
                return;
            }
            ether_output(dev, ipv4_mbuf, arp_entry->mac_addr, ETHER_TYPE_IP);
        }
    }
}