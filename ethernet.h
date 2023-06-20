#ifndef ETHERNET_H
#define ETHERNET_H

#define ETHER_TYPE_IP 0x0800
#define ETHER_TYPE_ARP 0x0806
#define ETHER_TYPE_IPV6 0x86dd

void ether_input(unsigned char *);

#endif // ETHERNET_H