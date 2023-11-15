CC=g++
CFLAGS=-g -Wall
SRC=src
OBJ=obj
BINDIR=bin
BIN=$(BINDIR)/engine
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
TESTBINS=$(patsubst $(TEST)/)
RM = rm

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BINDIR)/* $(OBJ)/*

