# Particle World: Cellular particle simulator
<h3>The project was inspired by Noita game</h3>

<h4>Demos</h4>
<img src="resources/demo1.gif">
<img src="resources/demo2.gif">
<img src="resources/demo3.gif">

-----------------
<h4>Build and Run</h4>

<h4>Windows</h4>

Download SDL2 MINGW binaries from https://github.com/libsdl-org/SDL/releases?q=2.&expanded=true#release

From downloaded zip file

Copy file `bin/SDL2.dll` to the project's root directory

Copy files from `include/SDL2` to `third_party/include/SDL2` directory

Copy `.a` library files from `lib/` to `third_party/lib/SDL2/` directory

Build project using either

```
make windows
```
or
```
build.bat
```

<h4>Linux</h4>

Install SDL2

https://wiki.libsdl.org/SDL2/Installation

Build project using either

```
make linux
```
or
```
./build.sh
```