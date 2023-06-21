#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "net.h"
#include "ethernet.h"
#include "ip.h"

#define IGNORE_INTERFACES                    \
  {                                          \
    "lo", "bond0", "dummy0", "tunl0", "sit0" \
  }
#define buffer_size 1500

void buffer_init(unsigned char *buffer)
{
  for (int i = 0; i < buffer_size; i++)
  {
    buffer[i] = 0;
  }
}

bool is_ignore_interface(char *ifname)
{
  char ignore_interfaces[][IF_NAMESIZE] = IGNORE_INTERFACES;
  for (int i = 0; i < sizeof(ignore_interfaces) / IF_NAMESIZE; i++)
  {
    if (strcmp(ignore_interfaces[i], ifname) == 0)
    {
      return true;
    }
  }
  return false;
}

struct net_device_data
{
  int fd;
};

int net_device_poll(struct net_device *);

int main(int argc, char *argv[])
{
  int socketfd;
  struct ifreq ifreq;
  struct ifaddrs *ifaddrs;
  getifaddrs(&ifaddrs);

  for (struct ifaddrs *tmp = ifaddrs; tmp != NULL; tmp = tmp->ifa_next)
  {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
    {
      memset(&ifreq, 0, sizeof(struct ifreq));
      strcpy(ifreq.ifr_name, tmp->ifa_name);
      if (is_ignore_interface(tmp->ifa_name))
      {
        printf("Skipped to enable interface %s\n", tmp->ifa_name);
        continue;
      }

      if ((socketfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
      {
        perror("socket failed\n");
        exit(1);
      }

      if (ioctl(socketfd, SIOCGIFINDEX, &ifreq) < 0)
      {
        fprintf(stderr, "ioctl SIOCGIFINDEX faield\n");
        close(socketfd);
        exit(1);
      }

      struct sockaddr_ll addr;
      memset(&addr, 0, sizeof(addr));
      addr.sll_family = AF_PACKET;
      addr.sll_protocol = htons(ETH_P_ALL);
      addr.sll_ifindex = ifreq.ifr_ifru.ifru_ivalue;
      if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
      {
        fprintf(stderr, "bind failed\n");
        close(socketfd);
        exit(1);
      }

      if (ioctl(socketfd, SIOCGIFHWADDR, &ifreq) != 0)
      {
        printf("%s\n", "ioctl SIOCGIFHWADDR failed");
        close(socketfd);
        continue;
      }

      struct net_device *dev = (struct net_device *)calloc(1, sizeof(struct net_device) + sizeof(struct net_device_data));
      // dev->ops.transmit = net_device_transmit;
      dev->ops.poll = net_device_poll;
      strcpy(dev->name, tmp->ifa_name);
      memcpy(dev->mac_addr, &ifreq.ifr_ifru.ifru_hwaddr.sa_data[0], 6);
      ((struct net_device_data *)dev->data)->fd = socketfd;

      printf("Created device %s socket %d\n", dev->name, socketfd);

      struct net_device *next;
      next = dev_base;
      dev_base = dev;
      dev->next = next;

      // non blocking mode
      int val = fcntl(socketfd, F_GETFL, 0);
      fcntl(socketfd, F_SETFL, val | O_NONBLOCK);
    }
  }

  freeifaddrs(ifaddrs);

  if (dev_base == NULL)
  {
    fprintf(stderr, "No interfaces");
    exit(1);
  }

  while (true)
  {
    for (struct net_device *dev = dev_base; dev; dev = dev->next)
    {
      dev->ops.poll(dev);
    }
  }

  close(socketfd);
  freeifaddrs(ifaddrs);
  return 0;
}

int net_device_poll(struct net_device *dev)
{
  ssize_t n;
  unsigned char buffer[buffer_size];
  n = recv(((struct net_device_data *)dev->data)->fd, buffer, sizeof(buffer), 0);
  if (n == -1)
  {
    if (errno == EAGAIN)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  }
  printf("Received %lu bytes from %s: ", n, dev->name);
  for (int i = 0; i < n; i++)
  {
    printf("%02X", buffer[i]);
  }
  printf("\n");
  return 0;
}
