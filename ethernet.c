#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include "ethernet.h"
#include "ip.h"
#include "arp.h"
#include "net.h"
#include "mbuf.h"

struct ethernet_header
{
    uint8_t dest_mac_addr[6];
    uint8_t source_mac_addr[6];
    uint16_t ethertype;
} __attribute__((__packed__));

void ether_dump(unsigned char *buffer)
{
    struct ethernet_header *ether_header;
    ether_header = (struct ethernet_header *)buffer;
    uint16_t ether_type = ntohs(ether_header->ethertype);

    printf("Ethernet Header\n");
    printf("Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ether_header->dest_mac_addr[0], ether_header->dest_mac_addr[1], ether_header->dest_mac_addr[2], ether_header->dest_mac_addr[3], ether_header->dest_mac_addr[4], ether_header->dest_mac_addr[5]);
    printf("Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ether_header->source_mac_addr[0], ether_header->source_mac_addr[1], ether_header->source_mac_addr[2], ether_header->source_mac_addr[3], ether_header->source_mac_addr[4], ether_header->source_mac_addr[5]);
    printf("Type: %04X\n", ether_type);

    switch (ether_type)
    {
    case ETHER_TYPE_ARP:
        printf("%s\n", "ARP!");
        arp_dump(buffer + sizeof(struct ethernet_header));
        return;
    case ETHER_TYPE_IP:
        ipv4_dump(buffer + sizeof(struct ethernet_header));
        return;
    case ETHER_TYPE_IPV6:
        printf("%s\n", "IPV6!");
        return;
    default:
        printf("%s\n", "Unknown Ethernet type");
        break;
    }
}

void ether_input(struct net_device *dev, unsigned char *buffer, ssize_t len) {
    struct ethernet_header *ether_header;
    ether_header = (struct ethernet_header *)buffer;
    uint16_t ether_type = ntohs(ether_header->ethertype);
     switch (ether_type)
    {
    case ETHER_TYPE_ARP:
        arp_dump(buffer + sizeof(struct ethernet_header));
        return;
    case ETHER_TYPE_IP:
        printf("%s\n", "IPV4");
        ipv4_input(dev, buffer + sizeof(struct ethernet_header), len - sizeof(struct ethernet_header));
        return;
    case ETHER_TYPE_IPV6:
        printf("%s\n", "IPV6");
        return;
    default:
        printf("%s\n", "Unknown Ethernet type");
        return;
    }

    return;
}

void ether_output(struct net_device *dev, struct mbuf *payload, uint8_t *dest_addr, uint16_t ether_type) {
    struct mbuf *ether_mbuf = create(sizeof(struct ethernet_header));
    struct ethernet_header *ether_header;
    ether_header = (struct ethernet_header *)ether_mbuf->buffer;
    
    memcpy(ether_header->source_mac_addr, dev->mac_addr, 6);
    memcpy(ether_header->dest_mac_addr, dest_addr, 6);
    ether_header->ethertype = htons(ether_type);

    payload->prev = ether_mbuf;
    ether_mbuf->next = payload;

    uint8_t buffer[1500];
    size_t total_len = 0;
    struct mbuf *current = ether_mbuf;
    while(current != NULL) {
        if(total_len + current->len > sizeof(buffer)) {
            fprintf(stderr, "buffer overflow\n");
            exit(1);
        }
        memcpy(&buffer[total_len], current->buffer, current->len);
        total_len += current->len;
        current = current->next;
    }

    dev->ops.transmit(dev, buffer, total_len);

    // @TODO Free mbuf
}