#!/bin/bash

C_COMPILER=gcc
FLAGS="-O3"
THIRDPARTY_INCLUDE_FOLDER="-Ithird_party/include"
LIB_FOLDER="-Lthird_party/lib"
LINUX_LINKER_LIBS="-lSDL2 -lm"
OUTPUT_NAME="linux_build"

PROJECT_INCLUDES=""
SRC_FILES=""

while IFS= read -r file; do
    SRC_FILES="$SRC_FILES $file"
done < <(find src -type f -name "*.c")

while IFS= read -r dir; do
    PROJECT_INCLUDES="$PROJECT_INCLUDES -I$dir"
done < <(find src -type d)

PROJECT_INCLUDES="-Isrc $PROJECT_INCLUDES"

START=$(date +%s.%N)

$C_COMPILER \
    $FLAGS \
    $THIRDPARTY_INCLUDE_FOLDER \
    $PROJECT_INCLUDES \
    $LIB_FOLDER \
    -o $OUTPUT_NAME \
    $SRC_FILES \
    $LINUX_LINKER_LIBS

END=$(date +%s.%N)

DURATION=$(echo "$END - $START" | bc)

echo "Build finished in ${DURATION}s"