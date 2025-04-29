CPP_COMPILER = g++
C_COMPILER = gcc
INCLUDE_FOLDER = -Isrc/include
LIB_FOLDER = -Lsrc/lib
LINKER_LIBS = -lmingw32 -lSDL2main -lSDL2 -lm


main: main.c
	$(C_COMPILER) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)

all: main
	./main

clean:
	del main.exe