# Makefile for main.c
main: main.o ethernet.o ip.o arp.o utils.o net.o mbuf.o
	gcc -Wall -O2 -o main main.o ethernet.o ip.o arp.o utils.o net.o mbuf.o

.PHONY: clean
clean:
	rm -rf ./*.o

.c.o:
	gcc -c $<