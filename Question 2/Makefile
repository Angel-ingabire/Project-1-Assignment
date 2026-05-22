CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = bus_navigator

SRCS = main.c bus_route.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c bus_route.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) bus_navigator.exe
