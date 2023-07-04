#ifndef IP_H
#define IP_H

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_REQUEST 8
#define ICMP_PROTOCOL_NUM 1

void icmp_dump(unsigned char *);
void icmp_input(uint32_t, uint32_t, unsigned char *, ssize_t);

#endif // IP_H