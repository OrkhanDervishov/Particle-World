@echo off
setlocal enabledelayedexpansion

set C_COMPILER=gcc
set FLAGS=-O3
set THIRDPARTY_INCLUDE_FOLDER=-Ithird_party/include
set LIB_FOLDER=-Lthird_party/lib
set WINDOWS_LINKER_LIBS=-lmingw32 -lSDL2main -lSDL2 -lm
set OUTPUT_NAME=windows_build

set PROJECT_INLUDES=
set SRC_FILES=

for /R src %%f in (*.c) do (
    set SRC_FILES=!SRC_FILES! %%f
)

for /D /R src %%d in (*) do (
    set PROJECT_INLUDES=!PROJECT_INLUDES! -I%%d
)

set PROJECT_INLUDES=-Isrc !PROJECT_INLUDES!

powershell -Command ^
"(Measure-Command { %C_COMPILER% %FLAGS% %THIRDPARTY_INCLUDE_FOLDER% %PROJECT_INLUDES% %LIB_FOLDER% -o %OUTPUT_NAME% %SRC_FILES% %WINDOWS_LINKER_LIBS% }).TotalMilliseconds" 

echo Build finished
pause