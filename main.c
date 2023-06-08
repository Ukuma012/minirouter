#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
  int socketfd;
  if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket failed\n");
    exit(1);
  }
  
  close(socketfd);
  return 0;
}
