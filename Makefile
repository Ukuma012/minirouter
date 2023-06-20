# Makefile for main.c
main: main.o ethernet.o ip.o arp.o utils.o net.o
	gcc -Wall -O2 -o main main.o ethernet.o ip.o arp.o utils.o net.o

.c.o:
	gcc -c $<