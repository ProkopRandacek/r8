GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast
LIBS = glfw3 gl
GCC_LIB = $(shell pkg-config --libs $(LIBS)) -lm -ldl
GCC_INCLUDES = -Iinclude/

build: clean shaders glad
	mkdir build -p
	cp src/*.scene build/
	gcc src/*.c -o build/rendering $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB)

run: clean build
	cd build/; ./rendering

shaders: clean
	cat $(wildcard src/shaders/*frag.glsl) > fragFull.glsl
	cat $(wildcard src/shaders/*vert.glsl) > vertFull.glsl
	mono shader_minifier.exe fragFull.glsl --preserve-externals -o src/frag.h
	mono shader_minifier.exe vertFull.glsl --preserve-externals -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f

# I don't want to have glad cource in plan text because github then shows random +6000 lines of code in statistics
glad:
	unzip -o glad.zip

clean:
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
	rm src/vert.h src/frag.h src/gl.c -f
	rm include -rf
