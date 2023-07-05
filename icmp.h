#ifndef IP_H
#define IP_H

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_REQUEST 8
#define ICMP_PROTOCOL_NUM 1

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

void icmp_dump(unsigned char *);
void icmp_input(uint32_t, uint32_t, unsigned char *, ssize_t);

#endif // IP_H