#ifndef ARP_H
#define ARP_H

struct net_device;

struct arp_entry
{
    uint8_t mac_addr[6];
    uint32_t ip_addr;
    struct net_device *dev;
    struct arp_entry *next;
};

void arp_cash_add(struct net_device *, uint8_t *, uint32_t ip_addr);
void arp_dump(unsigned char *);
void arp_input(struct net_device *, unsigned char *, ssize_t);

#endif // ARP_H