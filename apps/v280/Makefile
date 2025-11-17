CC 			?= gcc
CFLAGS 	?= -Wall -Wextra -I../../lib/v280
LDLIBS 	?= -lV120

TARGET 	?= run_v280
SRCS 		?= run_v280.c ../../lib/v280/v280.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)