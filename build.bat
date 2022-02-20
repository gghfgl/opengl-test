@echo off

if not defined DevEnvDir (
   call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

set GLFW3LIB="..\lib\glfw-3.3.6\build\src\Debug"
set GLFW3INC="..\lib\glfw-3.3.6\include"
set GLADINC="..\lib\glad-4.5.0\include"
set GLADSRC="..\lib\glad-4.5.0\src\glad.c"
set STBSRC="..\lib\stb"
set GLMINC="..\lib\glm"

mkdir build
pushd build

cl /EHa -FC -WX -W4 -wd4201 -wd4100 -wd4505 -wd4189 -MTd -Zi "..\src\main.cpp" %GLADSRC% /I %GLFW3INC% /I %GLADINC% /I %GLMINC% /I %STBSRC% /link /LIBPATH:%GLFW3LIB% user32.lib gdi32.lib shell32.lib opengl32.lib glfw3.lib

popd
