CC 			?= gcc
CFLAGS = -Wall -Wextra

OBJS = v210.o

.PHONY: all clean

all: $(OBJS)

v210.o: v210.c v210.h v210_reg.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)