#!/bin/make
#Amit Sarussi sucks

CC = gcc
CFLAGS = -Wall
_OBJ = i_config.o i_enviroment.o random_choice.o
DEPS = i_config.c i_enviroment.c random_choice.c

randenv: $(_OBJ) 
	$(CC) -c $(DEPS) $(CFLAGS)
	$(CC) src.c $(_OBJ) -o $@ $(CFLAGS)
	
.PHONY: all randenv clean

clean:
	rm -f *.o
