@echo off

echo Building x64 Release
echo ------------------
echo This is optimized release build, expect longer compile-time...

setlocal EnableDelayedExpansion
set output=bin\build

set include_dir=-I./include -I./src/headers
set lib_dir=-L./lib
set libs=-lopengl32 -lglew32 -lmingw32 -lsdl2main -lsdl2

for /f %%A in ('forfiles /s /m *.cpp /p src /c "cmd /c echo @relpath"') do set var=!var! .\src\%%~A

set total=-std=c++11 -Wall %var% %include_dir% %lib_dir% %libs% -o %output% -O3 -mwindows

echo Compile options:
echo %total%

@echo on

@g++ %total%

@echo ------------------
@echo Finished build.exe