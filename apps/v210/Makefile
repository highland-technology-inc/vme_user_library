CC 			?= gcc
CFLAGS 	?= -Wall -Wextra -I../../lib/v210
LDLIBS 	?= -lV120

TARGET 	?= run_v210
SRCS 		?= run_v210.c ../../lib/v210/v210.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)