CC 			?= gcc
CFLAGS = -Wall -Wextra

OBJS = v280.o

.PHONY: all clean

all: $(OBJS)

v280.o: v280.c v280.h v280_reg.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)