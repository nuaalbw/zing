BIN_TARGET = echo
CC = g++
SRCS=$(wildcard *.cpp)
CFLAGS = -Wall -g
LDFLAGS = -L../../src/libs -I../../src/net -I../../src/base -lzing -lpthread

ALL:$(BIN_TARGET)

$(BIN_TARGET):$(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_TARGET)

.PHONY:clean ALL