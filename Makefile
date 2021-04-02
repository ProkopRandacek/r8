SOURCES = $(wildcard src/*.c) $(wildcard src/**/*.c)

GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized -Waggregate-return
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast #-g3
GCC_LIB = -lm -ldl -lpthread -DUMKA_STATIC
GCC_INCLUDES = -Iinclude/ -Isubmodules/glfw/include

MINGW = x86_64-w64-mingw32-gcc
MINGW_FLAGS = $(GCC_FLAGS)
MINGW_LIB = -lm -lopengl32 -lgdi32 -Wl,-Bstatic -lpthread -Llib/windows -DUMKA_STATIC -static

UMKA_LIB = submodules/umka/build/libumka.a
UMKA_LIB_WIN = submodules/libumka_static.a

GLFW_ARGS = -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_VULKAN_STATIC=OFF
GLFW_LIB = submodules/glfw/src/libglfw3.a
GLFW_LIB_WIN = submodules/libglfw3.a

NAME = r8
BUILD_NAME = $(shell git rev-parse --short HEAD)
BUILD_TIME = $(shell date +'%H%M%S_%d%m%y')
BUILD_DIR = build

.PHONY: umka all glfw scripts

build: deps clean glad glfw umka scripts
	# Build the Linux binary
	gcc $(SOURCES) $(UMKA_LIB) $(GLFW_LIB) -o $(BUILD_DIR)/$(NAME) $(GCC_INCLUDES) $(GCC_FLAGS) $(GCC_LIB) -DBUILD_NAME=\"$(BUILD_NAME)\"

package:
	# Making a package
	zip -r $(NAME)_$(BUILD_NAME)_$(BUILD_TIME).zip build/

scripts:
	# Copy scripts
	mkdir $(BUILD_DIR)/scripts -p
	cp scripts/* $(BUILD_DIR)/scripts/ -r

wbuild: deps clean glad scripts
	#Build the Windows binary
	$(MINGW) $(SOURCES) $(UMKA_LIB_WIN) $(GLFW_LIB_WIN) -o $(BUILD_DIR)/$(NAME).exe $(GCC_INCLUDES) $(MINGW_FLAGS) $(MINGW_LIB) -DBUILD_NAME=\"$(BUILD_NAME)\"
	echo ".\r8.exe > log.txt 2>&1" > $(BUILD_DIR)/run.bat
	touch $(BUILD_DIR)/log.txt

run: clean build
	# Run the binary
	cd $(BUILD_DIR)/ && ./$(NAME)

# I don't want to have 3rd praty source in plan text because github then shows random +20000 lines of code in statistics
glad:
	# Extract GLAD source
	unzip -oq glad.zip

umka:
	# Compile Umka library
	cd submodules/umka/ && $(MAKE)

glfw:
	# Compile GLFW
	cd submodules/glfw/ && cmake $(GLFW_ARGS) .
	cd submodules/glfw/ && $(MAKE)

deps:
	# Checking dependencies
	if test ! -f submodules/glfw/CMakeLists.txt || test ! -f submodules/umka/Makefile; then git submodule update --init --recursive; fi

clean:
	# Remove previous build files
	rm $(BUILD_DIR) include -rf
	rm full.frag full.vert src/vert.h src/frag.h src/gl.c -f

cleandeps:
	cd submodules/glfw && git checkout . && git clean -dfx
	cd submodules/umka && git checkout . && git clean -dfx
