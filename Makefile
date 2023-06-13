# Makefile for main.c
main: main.o ethernet.o
	gcc -Wall -O2 -o main main.o ethernet.o

.c.o:
	gcc -c $<