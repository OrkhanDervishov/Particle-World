CPP_COMPILER = g++
C_COMPILER = gcc
INCLUDE_FOLDER = -Ithird_party/include
LIB_FOLDER = -Lthird_party/lib
LINKER_LIBS = -lmingw32 -lSDL2main -lSDL2 -lm
# LINKER_LIBS = -lmingw32 -lm

C_TEST_FILES = src/main.c src/engine/core.c src/list.c src/engine/window.c

SRC = $(wildcard src/*.c src/engine/*.c src/engine/ds/*.c src/partsim/*.c src/structs/*.c)
TEST = $(wildcard chunk_test/*.c chunk_test/engine/*.c chunk_test/engine/ds/*.c)
# CNK_TEST = $(wildcard src/*.c src/engine/*.c src/engine/ds/*.c src/structs/*.c src/partsim/d)

#FLAGS = -Wall -Wextra

main: $(SRC)
	$(C_COMPILER) $(FLAGS) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

main1: $(CNK_TEST)
	$(C_COMPILER) $(FLAGS) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

test: $(TEST)
	$(C_COMPILER) $(FLAGS) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

run: test
	./test

all: main
	./main

clean:
	del main.exe
