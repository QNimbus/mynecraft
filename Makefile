UNAME = $(shell uname -s)
KERNEL := $(if $(findstring MINGW,$(UNAME)),MinGW,$(UNAME))

CC = gcc
CPP = g++
MAKE = make
CMAKE = cmake
CMAKEFLAGS = 
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CFLAGS += -Ilib/glad/include -Ilib/glfw/include

CPPFLAGS = -std=c++14 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CPPFLAGS += -Wno-pointer-arith -Wno-unused-parameter -Wno-unused
CPPFLAGS += -Ilib -Ilib/glad/include -Ilib/glfw/include

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a -lm
# LDFLAGS = lib/glad/src/glad.o lib/cglm/libcglm.a lib/glfw/src/libglfw3.a lib/noise/libnoise.a -lm

# GLFW required frameworks on OSX
ifeq ($(KERNEL), Darwin)
# LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

ifeq ($(KERNEL), Linux)
# LDFLAGS += -ldl -lpthread
endif

ifeq ($(KERNEL), MinGW)
	MAKE = mingw32-make.exe
	LDFLAGS += -lgdi32 -lopengl32
	CMAKEFLAGS += -G "MinGW Makefiles"
endif

# Powershell/Windows
ifeq ($(KERNEL), MSYS_NT-10.0-22000)
	MAKE = mingw32-make.exe
	LDFLAGS += -lgdi32 -lopengl32
	CMAKEFLAGS += -G "MinGW Makefiles"
endif

SRC  = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all clean execute run

all: dirs libs game

libs:
# cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
	cd lib/glad && $(CC) -o src/glad.o -Iinclude/ -c src/glad.c
	cd lib/glfw && $(CMAKE) $(CMAKEFLAGS) . && $(MAKE)
# cd lib/noise && make

dirs:
	mkdir -p ./$(BIN)

game: $(OBJ)
	$(CPP) -o $(BIN)/game $^ $(LDFLAGS)

run: all
	$(BIN)/game

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)