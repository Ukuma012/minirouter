#ifndef ARP_H
#define ARP_H

#define ARP_HARDWARETYPE_ETHERNET 0x0001
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002
struct net_device;
struct arp_header;

struct arp_entry
{
    uint8_t mac_addr[6];
    uint32_t ip_addr;
    struct net_device *dev;
    struct arp_entry *next;
};

void arp_cash_add(struct net_device *, uint8_t *, uint32_t);
struct arp_entry *arp_cash_search(uint32_t );
void arp_dump(unsigned char *);
void arp_input(struct net_device *, unsigned char *, ssize_t);
void arp_request_process(struct net_device *, struct arp_header *);
void arp_reply_process(struct net_device *, struct arp_header *);
void arp_request(struct net_device *, uint32_t);

#endif // ARP_H