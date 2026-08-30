CPP_COMPILER = g++
C_COMPILER = gcc
DEFINITIONS = -D PW_USE_IMGUI

COMPILER = 
FLAGS = -O3 -g
THIRDPARTY_INCLUDE_FOLDER = -Ithird_party/include
LIB_FOLDER = -Lthird_party/lib/SDL2 -Lthird_party/lib/cimgui
WINDOWS_LINKER_LIBS = -lmingw32 -lcimgui -lSDL2main -lSDL2 -lm -lopengl32
LINUX_LINKER_LIBS = -lSDL2 -lm -lopengl32


# rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
# SRC := $(call rwildcard,src/,*.c)

# OBJ = $(SRC:%.c=bin/%.o)

rwildir = $(foreach d,$(wildcard $1*/),$(call rwildir,$d) $d)


rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

C_SRC   := $(call rwildcard,src/,*.c)
CPP_SRC := $(call rwildcard,src/,*.cpp)

SRC := $(C_SRC) $(CPP_SRC)

# Object files
C_OBJ   := $(C_SRC:%.c=bin/%.o)
CPP_OBJ := $(CPP_SRC:%.cpp=bin/%.o)
OBJ = $(C_OBJ) $(CPP_OBJ)

BIN_DIR = bin


INCLUDE_DIRS := src/ $(call rwildir,src/)
PROJECT_INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

windows_build: $(SRC)
	$(CPP_COMPILER) $(DEFINITIONS) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(WINDOWS_LINKER_LIBS)

# win_build: $(OBJ)
# 	$(CPP_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(IMGUI_BACKENDS) $(WINDOWS_LINKER_LIBS)

win_build: $(OBJ)
	$(CPP_COMPILER) $(DEFINITIONS) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(WINDOWS_LINKER_LIBS)


linux_build: $(SRC)
	$(CPP_COMPILER) $(DEFINITION) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINUX_LINKER_LIBS)


lin_build: $(OBJ)
	$(CPP_COMPILER) $(DEFINITION) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) $(LIB_FOLDER) -o $@ $^ $(LINUX_LINKER_LIBS)

# bin/%.o: %.c
# 	@if not exist "$(dir $@)" mkdir "$(dir $@)"
# 	$(C_COMPILER) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) -c $< -o $@


bin/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(C_COMPILER) $(DEFINITIONS) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) -c $< -o $@

bin/%.o: %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CPP_COMPILER) $(DEFINITIONS) $(FLAGS) $(PROJECT_INCLUDES) $(THIRDPARTY_INCLUDE_FOLDER) -c $< -o $@



windows: windows_build
	./windows_build

all2: win_build
	./win_build

clean2:
	del /S /Q "./bin/" *.o

linux: linux_build
	./linux_build
