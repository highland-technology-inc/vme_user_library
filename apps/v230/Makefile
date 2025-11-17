CC 				?= gcc
V230_DASH ?= -DV230_21
CFLAGS 		= -Wall -Wextra -I../../lib/v230 $(V230_DASH)
LDLIBS 		?= -lV120

TARGET 	?= run_v230
SRCS 		?= run_v230.c ../../lib/v230/v230.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)