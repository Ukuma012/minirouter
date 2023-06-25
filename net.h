#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <stdio.h>

struct net_device;

struct net_device_ops{
    int (*transmit) (struct net_device *dev, uint8_t *buffer, size_t len);
    int (*poll) (struct net_device *dev);
};

struct ipv4_device;

struct net_device {
    char name[32];
    uint8_t mac_addr[6];
    struct net_device_ops ops;
    struct ipv4_device *ip_dev;
    struct net_device *next;
    uint8_t data[];
};

struct net_device * get_net_device(char *);

extern struct net_device *dev_base;
#endif // NET_H