CPP_COMPILER = g++
C_COMPILER = gcc
FLAGS = -O3
THIRDPARTY_INCLUDE_FOLDER = -Ithird_party/include
LIB_FOLDER = -Lthird_party/lib
WINDOWS_LINKER_LIBS = -lmingw32 -lSDL2main -lSDL2 -lm
LINUX_LINKER_LIBS = -lSDL2 -lm

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC := $(call rwildcard,src/,*.c)

OBJ = $(SRC:%.c=bin/%.o)

rwildir = $(foreach d,$(wildcard $1*/),$(call rwildir,$d) $d)
INCLUDE_DIRS := src/ $(call rwildir,src/)
PROJECT_INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

BIN_DIR = bin



windows_build: $(SRC)
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(WINDOWS_LINKER_LIBS)

win_build: $(OBJ)
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(WINDOWS_LINKER_LIBS)

linux_build: $(SRC)
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINUX_LINKER_LIBS)

bin/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) -c $< -o $@



windows: windows_build
	./windows_build

all2: win_build
	./win_build

linux: linux_build
	./linux_build
