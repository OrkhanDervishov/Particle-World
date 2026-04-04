CPP_COMPILER = g++
C_COMPILER = gcc
FLAGS = -O2
THIRDPARTY_INCLUDE_FOLDER = -Ithird_party/include
LIB_FOLDER = -Lthird_party/lib
LINKER_LIBS = -lmingw32 -lSDL2main -lSDL2 -lm
# LINKER_LIBS = -lmingw32 -lm

C_TEST_FILES = src/main.c src/engine/core.c src/list.c src/engine/window.c

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC := $(call rwildcard,src/,*.c)

OBJ = $(SRC:%.c=bin/%.o)

rwildir = $(foreach d,$(wildcard $1*/),$(call rwildir,$d) $d)
INCLUDE_DIRS := src/ $(call rwildir,src/)
PROJECT_INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

# SRC1 = $(wildcard src/*.c src/engine/*.c src/engine/ds/*.c src/partsim/*.c src/structs/*.c src/rendering/*.c src/input_system/*.c src/game/*.c src/gui/*.c)
# TEST = $(wildcard chunk_test/*.c chunk_test/engine/*.c chunk_test/engine/ds/*.c)
# CNK_TEST = $(wildcard src/*.c src/engine/*.c src/engine/ds/*.c src/structs/*.c src/partsim/d)


# main1: $(CNK_TEST)
# 	$(C_COMPILER) $(FLAGS) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

# test: $(TEST)
# 	$(C_COMPILER) $(FLAGS) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

# main: $(OBJ)
# 	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

BIN_DIR = bin

main: $(SRC)
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

main2: $(OBJ)
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

bin/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) -c $< -o $@



# run: test
# 	./test

all: main
	./main

all2: main2
	./main2



clean:
	del main.exe
