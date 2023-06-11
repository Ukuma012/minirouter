#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <linux/if_ether.h>

// TODO インターフェースを区別する

int main(int argc, char *argv[]) {
  struct ifaddrs *ifaddrs;
  getifaddrs(&ifaddrs);

  // for(struct ifaddrs *tmp = ifaddrs; tmp != NULL; tmp = tmp->ifa_next) {
  //   printf("Name: %s\n", tmp->ifa_name);
  //   if(tmp->ifa_addr == NULL) {
  //     continue;
  //   }
  //   printf("Address Family: %d\n", tmp->ifa_addr->sa_family);
  //   printf("-----------------\n");
  // }
  
  int socketfd;
  if((socketfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
    perror("socket failed\n");
    exit(1);
  }

  unsigned char buffer[1500];
  while(1) {
    ssize_t n;
    if((n = recv(socketfd, buffer, sizeof(buffer), 0)) < 0) {
      perror("recv failed\n");
      exit(1);
    }
    if(n != 0) {
      printf("Received %lu bytes:", n);
      for(int i = 0; i < n; i++) {
        printf("%02X", buffer[i]);
      }
      printf("\n");
    }
  }
  
  close(socketfd);
  freeifaddrs(ifaddrs);
  return 0;
}
