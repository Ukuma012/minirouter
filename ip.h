#ifndef IP_H
#define IP_H

#define ICMP_PROTOCOL 1
#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
#define IPV4_ADDRESS(a, b, c, d) (a * 0x1000000u + b * 0x10000 + c * 0x100 + d)
#define TTL_VALUE 64

struct ipv4_device
{
    uint32_t ipv4_address;
    uint32_t subnet_mask;
    uint32_t broadcast_addr;
};

struct net_device;

void ipv4_dump(unsigned char *);
void ipv4_address_set(struct net_device *, uint32_t, uint32_t);
void ipv4_input(struct net_device *, unsigned char *, ssize_t);
void ipv4_output(struct net_device *, struct mbuf *, uint32_t, uint32_t, uint8_t)

#endif // IP_H