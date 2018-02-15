GXX = g++
SFLAGS+= -I.
SFLAGS = -I./src
SFLAGS+= -I./src/headers
SFLAGS+= -I./include
SFLAGS+= -std=c++11 -Wall -Og -ggdb3
LFLAGS = -Og -ggdb3
LFLAGS+= -L./lib
LFLAGS+= -lglew32 -lopengl32 -lmingw32 -lsdl2main -lsdl2
SRC=$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJDIR=obj
OBJ=$(addprefix $(OBJDIR)/,$(SRC:%.cpp=%.o))

build:
	@echo -- Starting build --
	@mingw32-make link -j8 --silent

link: $(OBJ)
	@echo -- Linking --
	@$(GXX) $(OBJ) $(LFLAGS) -o bin/build

$(OBJDIR)/%.o: %.cpp %.hpp
	@echo -- Generating $@ --
	-mkdir $(subst /,\,./$(dir $@)) >nul 2>&1
	@$(GXX) $(SFLAGS) -c $(subst $(OBJDIR)/,,$<) -o $@

# - before command ignores errors
# Here we substitute forwardslashes for backwardslashes cuz of windows
# We also remove obj/ from path of each file to access sourcefiles from path to obj files
# ">nul 2>&1" after a command just redirects stderr to nul so we don't get output
$(OBJDIR)/%.o: %.cpp
	@echo -- Generating $@ --
	-mkdir $(subst /,\,./$(dir $@)) >nul 2>&1
	@$(GXX) $(SFLAGS) -c $(subst $(OBJDIR)/,,$<) -o $@

run: build
	@echo -- Running --
	@cd bin && build

clean:
	@echo -- Cleaning --
	-del /s /q *.o

	