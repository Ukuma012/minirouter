# Makefile for main.c
main: main.o ethernet.o ip.o arp.o
	gcc -Wall -O2 -o main main.o ethernet.o ip.o arp.o

.c.o:
	gcc -c $<