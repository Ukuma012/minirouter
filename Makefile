# Makefile for main.c
main: main.o ethernet.o ip.o
	gcc -Wall -O2 -o main main.o ethernet.o ip.o

.c.o:
	gcc -c $<