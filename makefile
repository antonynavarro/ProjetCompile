TARGET = tpcc

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -g -I$(INC_DIR)

LEX_SRC        = $(SRC_DIR)/tpc.lex
BISON_SRC      = $(SRC_DIR)/tpc-2024-2025.y
C_SRC          = $(SRC_DIR)/tree.c
COMPILATEUR_SRC= $(SRC_DIR)/compilateur.c
TABSYMBOLE_SRC = $(SRC_DIR)/tabsymbole.c
SEMANTIQUE_SRC = $(SRC_DIR)/semantique.c
GENCODE_SRC    = $(SRC_DIR)/codegen.c

OBJ = \
  $(OBJ_DIR)/lex.yy.o \
  $(OBJ_DIR)/y.tab.o \
  $(OBJ_DIR)/tree.o \
  $(OBJ_DIR)/compilateur.o \
  $(OBJ_DIR)/tabsymbole.o \
  $(OBJ_DIR)/semantique.o \
  $(OBJ_DIR)/codegen.o

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

$(OBJ_DIR)/tree.o: $(C_SRC) $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(C_SRC) -o $(OBJ_DIR)/tree.o

$(OBJ_DIR)/compilateur.o: $(COMPILATEUR_SRC) $(INC_DIR)/compilateur.h $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(COMPILATEUR_SRC) -o $(OBJ_DIR)/compilateur.o

$(OBJ_DIR)/tabsymbole.o: $(TABSYMBOLE_SRC) $(INC_DIR)/tabsymbole.h $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(TABSYMBOLE_SRC) -o $(OBJ_DIR)/tabsymbole.o

$(OBJ_DIR)/semantique.o: $(SEMANTIQUE_SRC) $(INC_DIR)/semantique.h $(INC_DIR)/tabsymbole.h $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(SEMANTIQUE_SRC) -o $(OBJ_DIR)/semantique.o

$(OBJ_DIR)/codegen.o: $(GENCODE_SRC) $(INC_DIR)/codegen.h $(INC_DIR)/compilateur.h $(INC_DIR)/tree.h
	$(CC) $(CFLAGS) -c $(GENCODE_SRC) -o $(OBJ_DIR)/codegen.o

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) _anonymous.asm

run: clean all
	./$(BIN_DIR)/$(TARGET) < test/good/big.tpc

test: all
	./run_tests.sh