CC 				?= gcc
V230_DASH ?= -DV230_21
CFLAGS 		= -Wall -Wextra $(V230_DASH)

OBJS = v230.o

.PHONY: all clean

all: $(OBJS)

v230.o: v230.c v230.h v230_reg.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)