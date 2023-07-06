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

void icmp_dump(unsigned char *buffer) {
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
            printf("%s\n", "ICMP ECHO REQUEST arrives"); 
            icmp_dump(buffer);
            struct mbuf *reply_mbuf = mbuf_create(len);
            struct icmp_message *icmp_reply_message;
            icmp_reply_message = (struct icmp_message *)reply_mbuf->buffer;
            icmp_reply_message->header.type = ICMP_ECHO_REPLY;
            icmp_reply_message->header.code = icmp_message->header.code; 
            icmp_reply_message->header.checksum = 0;
            icmp_reply_message->reply.identity = icmp_message->reply.identity;
            icmp_reply_message->reply.sequence = icmp_message->reply.sequence;
            memcpy(&icmp_reply_message->reply.data, icmp_message->reply.data, len - sizeof(struct icmp_header) - sizeof(struct icmp_echo_reply));
            icmp_reply_message->header.checksum = calculate_checksum(reply_mbuf->buffer, reply_mbuf->len);

            ipv4_output(reply_mbuf, destination_ip_addr, soruce_ip_addr, ICMP_PROTOCOL_NUM);
            break;
        default:
            break;
    }
    return;
}

void icmp_time_exceeded(uint32_t destination_ip_addr, uint32_t soruce_ip_addr, uint8_t code, void *error_ip_buffer, size_t len) {
    if(len < sizeof(struct ipv4_header) + 8) {
        return;
    }

    struct mbuf *time_exceeded_mbuf = mbuf_create(sizeof(struct ipv4_header) + sizeof(struct icmp_time_exceeded) + 8);
    struct icmp_message *icmp_time_exceeded_msg;
    icmp_time_exceeded_msg = (struct icmp_message *)time_exceeded_mbuf->buffer;

    icmp_time_exceeded_msg->header.type = ICMP_TIME_EXCEEEDED;
    icmp_time_exceeded_msg->header.code = code;
    icmp_time_exceeded_msg->header.checksum = 0;
    icmp_time_exceeded_msg->time_exceeded.unused = 0;
    memcpy(icmp_time_exceeded_msg->time_exceeded.data, error_ip_buffer, sizeof(struct ipv4_header)+8);
    icmp_time_exceeded_msg->header.checksum = calculate_checksum(time_exceeded_mbuf->buffer, time_exceeded_mbuf->len);

    ipv4_output(time_exceeded_mbuf, destination_ip_addr, soruce_ip_addr, ICMP_PROTOCOL_NUM);
    return;
}