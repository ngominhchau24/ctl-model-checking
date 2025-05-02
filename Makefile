CC = gcc
CFLAGS = -Wall -Wextra -g
DEPS = ctl_api.h
SRC = ctl_implementation.c ctl_main.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: theorem3_verifier

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

theorem3_verifier: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f theorem3_verifier *.o