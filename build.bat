@echo off

if not defined DevEnvDir (
   call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

set GLFW3LIB="..\lib\glfw3\x64"
set GLFW3INC="..\lib\glfw3\include"
set GLADINC="..\lib\glad\include"
set GLADSRC="..\lib\glad\src\glad.c"
set EXTRAINC="..\lib"

mkdir build
pushd build

cl /EHa -FC -WX -W4 -wd4201 -wd4100 -wd4505 -wd4189 -MTd -Zi %GLADSRC% "..\src\main.cpp" /I %GLFW3INC% /I %GLADINC% /I %EXTRAINC% /link /LIBPATH:%GLFW3LIB% user32.lib shell32.lib opengl32.lib glfw3.lib

popd
