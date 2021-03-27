LIBS = glfw3 gl

GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast
GCC_LIB = $(shell pkg-config --libs $(LIBS)) -lm -ldl
GCC_INCLUDES = -Iinclude/

GLFW_WIN_INCLUDE = -Iglfw/include/
GLFW_DLL = -L./glfw/lib-vc2015/glfw3.dll -lopengl32
#D = --no-renaming

UMKA_LIB = submodules/umka/build/libumka.a

NAME = r8

build: clean shaders glad umka
	mkdir build -p
	cp src/*.scene build/
	gcc src/*.c ${UMKA_LIB} -o build/${NAME} $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB)

# build for windows
wbuild: clean shaders glfw
	mkdir build -p
	cp src/*.scene build/
	x86_64-w64-mingw32-gcc src/*.c -o build/${NAME}.exe -D GLFW_DLL $(GLFW_WIN_INCLUDE) $(GLFW_DLL) $(GCC_INCLUDES) $(GCC_FLAGS)

run: clean build
	cd build/; ./rendering

shaders: clean
	cat $(wildcard shaders/*frag.glsl) > fragFull.glsl
	cat $(wildcard shaders/*vert.glsl) > vertFull.glsl
	mono glsl_minify.exe fragFull.glsl --preserve-externals ${D} -o src/frag.h
	mono glsl_minify.exe vertFull.glsl --preserve-externals ${D} -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f

# I don't want to have 3rd praty source in plan text because github then shows random +20000 lines of code in statistics
glad:
	unzip -oq glad.zip

umka:
	cd submodules/umka/; make

clean:
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
	rm src/vert.h src/frag.h src/gl.c -f
	rm include glfw -rf
	#rm submodules/umka/build -rf
