@echo off

echo Building x64 Debug
echo ------------------

setlocal EnableDelayedExpansion
set output=bin\build

set include_dir=-I./include -I./src/headers
set lib_dir=-L./lib
set libs=-lglew32 -lopengl32 -lmingw32 -lsdl2main -lsdl2

for /f %%A in ('forfiles /s /m *.cpp /p src /c "cmd /c echo @relpath"') do set var=!var! .\src\%%~A

set lib_cpp=.\include\imgui\imgui.cpp .\include\imgui\imgui_draw.cpp .\include\imgui\imgui_demo.cpp

set total=-std=c++11 -Wall %var% %lib_cpp% %include_dir% %lib_dir% %libs% -o %output% -ggdb3 -Og

echo Compile options:
echo %total%

@echo on

@g++ %total%

@echo ------------------
@echo Finished build.exe