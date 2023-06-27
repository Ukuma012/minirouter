#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "net.h"
#include "arp.h"
#include "utils.h"

#define ARP_CASH_SIZE 32

struct arp_entry arp_cash[ARP_CASH_SIZE];

struct arp_header
{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_len;
    uint8_t protocol_len;
    uint16_t operation_code; // ARP_request:1, ARP_reply:2
    uint8_t source_mac_addr[6];
    uint32_t source_protocol_addr;
    uint8_t target_mac_addr[6];
    uint32_t target_protocol_addr;
} __attribute__((__packed__));

void arp_cash_add(struct net_device *dev, uint8_t *mac_addr, uint32_t ip_addr)
{
    uint32_t index;
    index = ip_addr % ARP_CASH_SIZE;
    struct arp_entry *candidate = &arp_cash[index];

    if (candidate->ip_addr == 0 || candidate->ip_addr == ip_addr)
    {
        memcpy(candidate->mac_addr, mac_addr, 6);
        candidate->ip_addr = ip_addr;
        candidate->dev = dev;
        return;
    }

    while (candidate->next != NULL)
    {
        candidate = candidate->next;

        if (candidate->ip_addr == ip_addr)
        {
            memcpy(candidate->mac_addr, mac_addr, 6);
            candidate->ip_addr = ip_addr;
            candidate->dev = dev;
            return;
        }
    }

    struct arp_entry *tmp;
    if ((tmp = calloc(1, sizeof(struct arp_entry))) == NULL)
    {
        fprintf(stderr, "calloc failed\n");
        exit(1);
    }
    candidate->next = tmp;
    memcpy(tmp->mac_addr, mac_addr, 6);
    tmp->ip_addr = ip_addr;
    tmp->dev = dev;

    return;
}

struct arp_entry *arp_cash_search(uint32_t ip_addr) {
    uint32_t index;
    index = ip_addr % ARP_CASH_SIZE;
    struct arp_entry *candidate = &arp_cash[index];

    if(candidate->ip_addr == ip_addr) {
        return candidate;
    } else if(candidate->ip_addr == 0) {
        return NULL;
    }

    while(candidate->next != NULL) {
        candidate = candidate->next;
        if(candidate->ip_addr == ip_addr) {
            return candidate;
        }
    }

    return NULL;
}

void arp_dump(unsigned char *buffer)
{
    struct arp_header *arp_header;
    arp_header = (struct arp_header *)buffer;

    printf("%s\n", "ARP");
    printf("hardware type: %04X\n", ntohs(arp_header->hardware_type));
    printf("protocol type: %04X\n", ntohs(arp_header->protocol_type));
    printf("hardware length: %u\n", arp_header->hardware_len);
    printf("protocol length: %u\n", arp_header->protocol_len);
    printf("operation code: %04X\n", ntohs(arp_header->operation_code));
    printf("Source MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", arp_header->source_mac_addr[0], arp_header->source_mac_addr[1], arp_header->source_mac_addr[2], arp_header->source_mac_addr[3], arp_header->source_mac_addr[4], arp_header->source_mac_addr[5]);
    printf("Source IP address: ");
    ip_dot(arp_header->source_protocol_addr);
    printf("Target MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", arp_header->target_mac_addr[0], arp_header->target_mac_addr[1], arp_header->target_mac_addr[2], arp_header->target_mac_addr[3], arp_header->target_mac_addr[4], arp_header->target_mac_addr[5]);
    printf("Target IP address: ");
    ip_dot(arp_header->target_protocol_addr);

    return;
}

void arp_input(struct net_device *dev, unsigned char *buffer, ssize_t len)
{
    printf("%s\n", "hello");
    return;
}

void arp_request(struct net_device *dev, uint32_t ip_addr) {

}