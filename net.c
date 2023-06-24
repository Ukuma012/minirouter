#include <string.h>
#include "net.h"

struct net_device *dev_base;

struct net_device * get_net_device(char *device_name) {
    for(struct net_device *dev = dev_base; dev; dev = dev->next) {
        if((strcmp(dev->name, device_name)) == 0) {
            return dev;
        }
    }
    return NULL;
}