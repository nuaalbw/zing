LIB_SRCS1=$(wildcard base/*.cpp)
LIB_SRCS2=$(wildcard net/*.cpp)
LIB_NAME=zing

CC = g++
CFLAGS=-Wall -std=c++11
LDFLAGS=-lpthread

ALL:static_lib shared_lib

static_lib:$(LIB_SRCS1) $(LIB_SRCS2)
	$(CC) -c $^ $(CFLAGS) $(LDFLAGS)
	ar -rcs lib$(LIB_NAME).a *.o
	rm -rf *.o

shared_lib:$(LIB_SRCS1) $(LIB_SRCS2)
	$(CC) -fPIC -shared $^ -o lib$(LIB_NAME).so

clean:
	rm -rf lib$(LIB_NAME).a lib$(LIB_NAME).so

.PHONY:clean ALL