LIBS = glfw3 gl

GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast
GCC_LIB = -lm -ldl -lpthread
GCC_INCLUDES = -Iinclude/ -Isubmodules/glfw/include

#D = --no-renaming

UMKA_LIB = submodules/umka/build/libumka.a

GLFW_LIB = submodules/glfw/src/libglfw3.a
GLFW_ARGS = -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_VULKAN_STATIC=OFF

NAME = r8

.PHONY: umka

build: deps clean shaders glad umka
	# Build the Linux binary
	mkdir build/scripts -p
	cp scripts/* build/scripts/ -r
	gcc src/*.c ${UMKA_LIB} ${GLFW_LIB} -o build/${NAME} $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB)

run: clean build
	# Run the binary
	cd build/ && ./${NAME}

shaders: clean
	# Minify shaders
	cat $(wildcard shaders/*frag.glsl) > fragFull.glsl
	cat $(wildcard shaders/*vert.glsl) > vertFull.glsl
	mono glsl_minify.exe fragFull.glsl --preserve-externals ${D} -o src/frag.h
	mono glsl_minify.exe vertFull.glsl --preserve-externals ${D} -o src/vert.h
	rm fragFull.glsl vertFull.glsl -f

# I don't want to have 3rd praty source in plan text because github then shows random +20000 lines of code in statistics
glad:
	# Extract glad source
	unzip -oq glad.zip

umka:
	# Compile umka library
	cd submodules/umka/ && ${MAKE}

glfw:
	cd submodules/glfw/ && cmake ${GLFW_ARGS} .
	cd submodules/glfw/ && ${MAKE}

deps:
	if test ! -f submodules/glfw/CMakeLists.txt; then git submodule update --init --recursive; fi
	if test ! -f submodules/umka/Makefile;       then git submodule update --init --recursive; fi

clean:
	# Remove previous build files
	rm build/ -rf
	rm fragFull.glsl vertFull.glsl -f
	rm src/vert.h src/frag.h src/gl.c -f
	rm include glfw -rf
