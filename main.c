#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <stdbool.h>
#include <string.h>
#include "ethernet.h"
#include "ip.h"

#define IGNORE_INTERFACES {"lo", "bond0", "dummy0", "tunl0", "sit0"}
#define buffer_size 1500

void buffer_init(unsigned char *buffer) {
  for(int i = 0; i < buffer_size; i++) {
    buffer[i] = 0;
  }
}

bool is_ignore_interface(char *ifname) {
  char ignore_interfaces[][IF_NAMESIZE] = IGNORE_INTERFACES;
  for(int i = 0; i < sizoeof(ignore_interfaces) / IF_NAMESIZE; i++) {
    if(strcmp(ignore_interfaces[i], ifname) == 0) {
      return true;
    }
  }
  return false;
}

int main(int argc, char *argv[])
{
  struct ifaddrs *ifaddrs;
  getifaddrs(&ifaddrs);

  for(struct ifaddrs *tmp = ifaddrs; tmp != NULL; tmp = tmp->ifa_next) {

  }

  int socketfd;
  if ((socketfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    perror("socket failed\n");
    exit(1);
  }

  unsigned char buffer[buffer_size];
  while (1)
  {
    ssize_t n;
    buffer_init(buffer);
    if ((n = recv(socketfd, buffer, sizeof(buffer), 0)) < 0)
    {
      perror("recv failed\n");
      exit(1);
    }
    if (n != 0)
    {
      printf("Received %lu bytes:", n);
      for (int i = 0; i < n; i++)
      {
        printf("%02X", buffer[i]);
      }
      printf("\n");
    }
    ether_input(buffer);
  }

  close(socketfd);
  freeifaddrs(ifaddrs);
  return 0;
}
