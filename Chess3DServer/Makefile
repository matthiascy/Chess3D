TARGET = GameServer
OBJS = main.o

LIBS =
INC =
LIB =

CC = gcc
cXX = g++
CFLAGS = -g -c -pipe -Wall
LDFLAGS = $(LIB)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) &(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -rf $(OBJS) $(TARGET)

SRC_C_FILE = $(@F:.o = .c)
SRC_CPP_FILE = $(@F:.o = .cpp)

.SUFFIXES:
.SUFFIXES: .o .c .cc .cpp

.c.o:
	$(CC) $(CFLAGS) $(INC) $(SRC_C_FILE)
.cc.o:
	$(CXX) $(CFLAGS) $(INC) $(SRC_CPP_FILE)
.cpp.o:
	$(CXX) $(CFLAGS) $(INC) $(SRC_CPP_FILE)
main.o: main.cpp file_define.h
