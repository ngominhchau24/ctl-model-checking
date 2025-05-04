# Makefile for the CTL Verifier

CC = gcc
CFLAGS = -Wall -Wextra -g

all: ctl_verifier

ctl_verifier: ctl_verifier.c
	$(CC) $(CFLAGS) -o ctl_verifier ctl_verifier.c

clean:
	rm -f ctl_verifier
