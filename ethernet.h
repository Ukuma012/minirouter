#ifndef ETHERNET_H
#define ETHERNET_H

struct net_device;
struct mbuf;

#define ETHER_ADDR_LENGTH 6
#define ETHER_TYPE_IP 0x0800
#define ETHER_TYPE_ARP 0x0806
#define ETHER_TYPE_IPV6 0x86dd

void ether_input(struct net_device *, unsigned char *, ssize_t);
void ether_output(struct net_device *, struct mbuf *, const uint8_t *, uint16_t);
void ether_dump(unsigned char *);

#endif // ETHERNET_H