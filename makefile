TARGET = tpcas

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -g -I$(INC_DIR)

LEX_SRC = $(SRC_DIR)/tpc.lex
BISON_SRC = $(SRC_DIR)/tpc-2024-2025.y
C_SRC = $(SRC_DIR)/tree.c
COMPILATEUR_SRC = $(SRC_DIR)/compilateur.c

OBJ = $(OBJ_DIR)/lex.yy.o $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/tree.o $(OBJ_DIR)/compilateur.o

all: $(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/$(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/y.tab.h $(OBJ_DIR)/y.tab.c: $(BISON_SRC)
	$(BISON) -d -o $(OBJ_DIR)/y.tab.c $(BISON_SRC)

$(OBJ_DIR)/lex.yy.o: $(LEX_SRC) $(OBJ_DIR)/y.tab.h
	$(FLEX) -o $(OBJ_DIR)/lex.yy.c $(LEX_SRC)
	$(CC) $(CFLAGS) -c $(OBJ_DIR)/lex.yy.c -o $(OBJ_DIR)/lex.yy.o

$(OBJ_DIR)/y.tab.o: $(OBJ_DIR)/y.tab.c
	$(CC) $(CFLAGS) -c $(OBJ_DIR)/y.tab.c -o $(OBJ_DIR)/y.tab.o

$(OBJ_DIR)/tree.o: $(SRC_DIR)/tree.c $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/tree.c -o $(OBJ_DIR)/tree.o

$(OBJ_DIR)/compilateur.o: $(SRC_DIR)/compilateur.c $(INC_DIR)/compilateur.h $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/compilateur.c -o $(OBJ_DIR)/compilateur.o

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: clean all
	./$(BIN_DIR)/$(TARGET)  < test/good/big.tpc

test: all
	./run_tests.sh

