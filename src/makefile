DIR_INC = ./
DIR_SRC1 = ./base
DIR_SRC2 = ./net
DIR_OBJ = ./obj

SRCS = $(wildcard $(DIR_SRC)/*)
OBJS_1 = $(patsubst %.cpp, $(DIR_OBJ)/%.o, $(filter %.cpp, $(notdir $(SRCS))))
OBJS_2 = $(patsubst %.cc, $(DIR_OBJ)/%.o, $(filter %.cc, $(notdir $(SRCS))))

CC = g++
CFLAGS = -Wall -g -I$(DIR_INC) -std=c++11 -lpthread -lprotobuf -ljson -lcrypto -lmysqlclient

ALL:$(BIN_TARGET)

$(BIN_TARGET):$(OBJS_1) $(OBJS_2)
	$(CC) $(OBJS_1) $(OBJS_2) $(CFLAGS) -o $@ 

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@	

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@	

clean:
	rm -rf $(DIR_OBJ)/*.o $(BIN_TARGET)

.PHONY:clean ALL
