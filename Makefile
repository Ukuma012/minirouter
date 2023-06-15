# Makefile for main.c
main: main.o ethernet.o ip.o arp.o utils.o
	gcc -Wall -O2 -o main main.o ethernet.o ip.o arp.o utils.o

.c.o:
	gcc -c $<