CPP_COMPILER = g++
C_COMPILER = gcc
INCLUDE_FOLDER = -I../src/include
LIB_FOLDER = -L../src/lib
LINKER_LIBS = -lmingw32 -lSDL2main -lSDL2 -lm


main: partsimulator.c
	$(C_COMPILER) $(INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINKER_LIBS)


all: main
	./main

clean:
	del main.exe