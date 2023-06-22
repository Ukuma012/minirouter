#include "ethernet.h"
#include "ip.h"
#include "arp.h"
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

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

void ether_input(struct net_device *dev, unsigned char *buffer, ssize_t len)
{
    struct ethernet_header *ethernet_header;
    ethernet_header = (struct ethernet_header *)buffer;

    return;
}