#ifndef IP_H
#define IP_H

#define ICMP_PROTOCOL 1
#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
#define IPV4_ADDRESS(a, b, c, d) (a * 0x1000000u + b * 0x10000 + c * 0x100 + d)

struct ipv4_device {
    uint32_t ipv4_address;
    uint32_t subnet_mask;
    uint32_t broadcast_addr;
};

struct net_device;

void ipv4_dump(unsigned char *);
void ipv4_address_set(struct net_device *dev, uint32_t ipv4_address, uint32_t subnet_mask);
void ipv4_input(struct net_device * dev, unsigned char * buffer, ssize_t len);

#endif // IP_H