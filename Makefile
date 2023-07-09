CC = gcc
CFLAGS = -Wall -O2

SRCS = main.c ethernet.c ip.c arp.c utils.c net.c mbuf.c icmp.c routing.c 
OBJS = $(SRCS:.c=.o)
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

