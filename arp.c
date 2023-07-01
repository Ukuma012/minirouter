#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "ethernet.h"
#include "ip.h"
#include "net.h"
#include "arp.h"
#include "utils.h"
#include "mbuf.h"

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

void arp_input(struct net_device *input_dev, unsigned char *buffer, ssize_t len)
{
    if(len < sizeof(struct arp_header)) {
        printf("%s\n", "packet is too short for arp");
        return;
    }

    struct arp_header *arp_input_header;
    arp_input_header = (struct arp_header *)buffer;
    
    uint16_t op_code = ntohs(arp_input_header->operation_code);

    switch (ntohs(arp_input_header->protocol_type))
    {
    case ETHER_TYPE_IP:
        if(op_code == ARP_REQUEST) {
            printf("%s\n", "ARP request arrives");
            arp_request_process(input_dev, arp_input_header);
            return;
        } else if(op_code == ARP_REPLY) {
            printf("%s\n", "ARP reply arrives");
            return;
        }
        break;
    }
}

void arp_request_process(struct net_device *input_dev, struct arp_header *request) {
    if(input_dev->ip_dev != NULL && input_dev->ip_dev->ipv4_address != IPV4_ADDRESS(0,0,0,0)) {
        if(input_dev->ip_dev->ipv4_address == ntohl(request->target_protocol_addr)) {
            struct mbuf *reply_mbuf;
            reply_mbuf = mbuf_create(sizeof(struct arp_header));
            
            struct arp_header *arp_reply_header;
            arp_reply_header = (struct arp_header *)reply_mbuf->buffer;

            arp_reply_header->hardware_type = htons(ARP_HARDWARETYPE_ETHERNET);
            arp_reply_header->protocol_type = htons(ETHER_TYPE_IP);
            arp_reply_header->hardware_len = ETHER_ADDR_LENGTH;
            arp_reply_header->protocol_len = IPV4_PROTOCOL_LENGTH;
            arp_reply_header->operation_code = htons(ARP_REPLY);
            memcpy(arp_reply_header->source_mac_addr, input_dev->mac_addr, 6);
            arp_reply_header->source_protocol_addr = htonl(input_dev->ip_dev->ipv4_address);
            memcpy(arp_reply_header->target_mac_addr, request->source_mac_addr, 6);
            arp_reply_header->target_protocol_addr = request->source_protocol_addr;

            ether_output(input_dev, reply_mbuf, request->source_mac_addr, ETHER_TYPE_ARP);
            arp_cash_add(input_dev, request->source_mac_addr, ntohl(request->target_protocol_addr));
            return;
        }
    }
}

void arp_reply_process(struct net_device *dev, struct arp_header *reply) {
    if(dev->ip_dev != NULL && dev->ip_dev->ipv4_address != IPV4_ADDRESS(0,0,0,0)) {
        arp_cash_add(dev, reply->source_mac_addr, ntohl(reply->source_protocol_addr));
    }
    return;
}

void arp_request(struct net_device *dev, uint32_t target_ip_addr) {
    struct mbuf *arp_mbuf;
    arp_mbuf = mbuf_create(sizeof(struct arp_header));
    struct arp_header *arp_header;
    arp_header = (struct arp_header *)arp_mbuf->buffer;

    arp_header->hardware_type = htons(ARP_HARDWARETYPE_ETHERNET);
    arp_header->protocol_type = htons(ETHER_TYPE_IP);
    arp_header->hardware_len = ETHER_ADDR_LENGTH;
    arp_header->protocol_len = IPV4_PROTOCOL_LENGTH;
    arp_header->operation_code = htons(ARP_REQUEST);
    memcpy(arp_header->source_mac_addr, dev->mac_addr, 6);
    arp_header->source_protocol_addr = htonl(dev->ip_dev->ipv4_address);
    arp_header->target_protocol_addr = htonl(target_ip_addr);

    const uint8_t ETHERNET_BROADCAST[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; 

    ether_output(dev, arp_mbuf, ETHERNET_BROADCAST, ETHER_TYPE_ARP);
}