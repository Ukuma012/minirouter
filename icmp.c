#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include "mbuf.h"
#include "utils.h"
#include "icmp.h"
#include "ip.h"

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
} __attribute__((packed));

struct icmp_echo_reply {
    uint16_t identity;
    uint16_t sequence;
    uint8_t data[];
} __attribute__((packed));

struct icmp_destination_unreachable {
    uint32_t unused;
    uint8_t data[];
} __attribute__((packed));

struct icmp_time_exceeded{
    uint32_t unused;
    uint8_t data[];
} __attribute__((packed));

struct icmp_message {
    struct icmp_header header;
    union{
    struct icmp_echo_reply reply;
    struct icmp_destination_unreachable dest_unreachable;
    struct icmp_time_exceeded time_exceeded;
    };
} __attribute__((packed));

void icmp_dump(unsigned char *buffer) {
    //@todo icmp_message cast
    struct icmp_message *icmp_message;
    icmp_message = (struct icmp_message *)buffer;

    printf("%s\n", "icmp dump start!"); 
    printf("icmp type: %u\n", icmp_message->header.type);
    printf("icmp code: %u\n", icmp_message->header.code);
    printf("icmp checksum: %u\n", icmp_message->header.checksum);
    printf("icmp identity: %u\n", icmp_message->reply.identity);
    printf("icmp sequence: %u\n", icmp_message->reply.sequence);

    return;
}

void icmp_input(uint32_t soruce_ip_addr, uint32_t destination_ip_addr, unsigned char *buffer, ssize_t len) {
    struct icmp_message *icmp_message;
    icmp_message = (struct icmp_message *)buffer;

    switch(icmp_message->header.type) {
        case ICMP_ECHO_REPLY:
            printf("%s\n", "ICMP ECHO REPLY arrives");
            return;
            
        case ICMP_ECHO_REQUEST:
            icmp_dump(buffer);
            printf("%s\n", "ICMP ECHO REQUEST arrives"); 

            struct mbuf *reply_mbuf = mbuf_create(len);
            struct icmp_message *icmp_reply_message;
            icmp_reply_message = (struct icmp_message *)reply_mbuf->buffer;

            // debug
            // printf("icmp checksum: %u\n", icmp_message->header.checksum);
            // printf("icmp identity: %u\n", icmp_message->reply.identity);
            // printf("icmp sequence: %u\n", icmp_message->reply.sequence);

            icmp_reply_message->header.type = ICMP_ECHO_REPLY;
            icmp_reply_message->header.code = icmp_message->header.code; 
            icmp_reply_message->header.checksum = 0;
            icmp_reply_message->reply.identity = icmp_message->reply.identity;
            icmp_reply_message->reply.sequence = icmp_message->reply.sequence;
            memcpy(&icmp_reply_message->reply.data, icmp_message->reply.data, len - sizeof(struct icmp_header) - sizeof(struct icmp_echo_reply));
            icmp_reply_message->header.checksum = calculate_checksum(reply_mbuf->buffer, reply_mbuf->len);

            // debug
            // printf("type: %d\n", icmp_reply_message->header.type);
            // printf("code: %d\n", icmp_reply_message->header.code);
            // printf("checksum: %u\n", icmp_reply_message->header.checksum);
            // printf("identity: %u\n", icmp_reply_message->reply.identity);
            // printf("sequence: %u\n", icmp_reply_message->reply.sequence);

            ipv4_output(reply_mbuf, soruce_ip_addr, destination_ip_addr, ICMP_PROTOCOL_NUM);
            break;
        default:
            break;
    }
    return;
}