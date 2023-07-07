#ifndef ICMP_H
#define ICMP_H

#define ICMP_ECHO_REPLY 0
#define ICMP_DESTINATION_UNREACHABLE 3
#define ICMP_ECHO_REQUEST 8
#define ICMP_TIME_EXCEEEDED 11
#define ICMP_PROTOCOL_NUM 1

#define NETWORK_UNREACHABLE 0
#define HOST_UNREACHABLE 1
#define PROTOCOL_UNREACHABLE 2
#define PORT_UNREACHABLE 3
#define FRAGMENTATION_NEEDED_AND_DF_SET 4
#define SOURCE_ROUTE_FAILED 5
#define DESTINATION_NETWORK_UNKNOEN 6

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
void icmp_time_exceeded(uint32_t, uint32_t, uint8_t, void *, size_t);
void icmp_destination_unreachable(uint32_t, uint32_t, uint8_t, void *, size_t);

#endif // ICMP_H