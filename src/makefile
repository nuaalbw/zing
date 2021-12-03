LIB_SRCS1=$(wildcard base/*.cpp)
LIB_SRCS2=$(wildcard net/*.cpp)
LIB_NAME=zing
LIB_PATH=libs

CC = g++
CFLAGS=-Wall -std=c++11
LDFLAGS=-lpthread

ALL:static_lib shared_lib install

static_lib:$(LIB_SRCS1) $(LIB_SRCS2)
	$(CC) -c $^ $(CFLAGS) $(LDFLAGS)
	ar -rcs lib$(LIB_NAME).a *.o
	rm -rf *.o

shared_lib:$(LIB_SRCS1) $(LIB_SRCS2)
	$(CC) -fPIC -shared $^ -o lib$(LIB_NAME).so

clean:
	rm -rf $(LIB_PATH)

install:
	mkdir $(LIB_PATH)
	mv lib$(LIB_NAME).* $(LIB_PATH)


.PHONY:clean ALL