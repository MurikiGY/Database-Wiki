CC = g++
CFLAGS = -g -Wall -O2
LDLIBS = -lm
SRC = $(wildcard ./sources/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)

debug: CFLAGS += -DDEBUG -g
debug: all

run: all
	./$(TARGET)

clean:
	-rm -f ./sources/*.o vgcore* 

purge: clean
	-rm -f $(TARGET)

