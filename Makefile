SRC = src
OBJ = obj

SOURCES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/**/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

GCC_WFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Winit-self -Wuninitialized -Waggregate-return -Wno-misleading-indentation
GCC_FLAGS = $(GCC_WFLAGS) -std=c11 -Ofast -g3
GCC_LIB = -lm -ldl -lpthread -DUMKA_STATIC
GCC_INCLUDES = -Iinclude/ -Isubmodules/glfw/include -I$(SRC) -I$(SRC)/umka -I$(SRC)/scene -I$(SRC)/shader

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

GCC_FULL   = $(GCC_INCLUDES) $(GCC_FLAGS)   $(GCC_LIB)   -DBUILD_NAME=\"$(BUILD_NAME)\"
MINGW_FULL = $(GCC_INCLUDES) $(MINGW_FLAGS) $(MINGW_LIB) -DBUILD_NAME=\"$(BUILD_NAME)\"

.PHONY: deps glfw umka scripts

all: deps glfw umka scripts build

build: $(OBJECTS)
	@echo Build the Linux binary
	gcc $(OBJECTS) $(UMKA_LIB) $(GLFW_LIB) $(GCC_FULL) -o $(BUILD_DIR)/$(NAME)
	@echo Build succeeded

$(OBJ)/%.o: $(SRC)/%.c
	gcc $(GCC_FULL) -c $< -o $@

package:
	@echo Making a package
	zip -r $(NAME)_$(BUILD_NAME)_$(BUILD_TIME).zip build/

graph:
	gcc $(SOURCES) -MM | ./submodules/gcc-deps-graph/graphMaker.py -i vector settings debug shapes camera
	rm ./deps.gv

scripts:
	@echo Copy scripts
	mkdir $(BUILD_DIR)/scripts $(OBJ)/shader $(OBJ)/scene $(OBJ)/umka -p
	cp scripts/* $(BUILD_DIR)/scripts/ -r

wbuild: deps clean scripts
	@echo Build the Windows binary
	$(MINGW) $(SOURCES) $(UMKA_LIB_WIN) $(GLFW_LIB_WIN) $(MINGW_FULL) -o $(BUILD_DIR)/$(NAME).exe
	echo ".\r8.exe > log.txt 2>&1" > $(BUILD_DIR)/run.bat
	touch $(BUILD_DIR)/log.txt
	@echo Build succeeded

run: all
	@echo Run the binary
	cd $(BUILD_DIR)/ && ./$(NAME)

umka:
	@echo Compile Umka library
	cd submodules/umka/ && $(MAKE)

glfw:
	@echo Compile GLFW
	cd submodules/glfw/ && cmake $(GLFW_ARGS) .
	cd submodules/glfw/ && $(MAKE)

deps:
	@echo Checking dependencies
	if test ! -f submodules/glfw/CMakeLists.txt || test ! -f submodules/umka/Makefile || test ! -f submodules/gcc-deps-graph/graphMaker.py; then git submodule update --init --recursive; fi

clean:
	@echo Remove previous build files
	rm $(OBJ) $(BUILD_DIR) -rf
	rm full.frag full.vert $(SRC)/vert.h $(SRC)/frag.h -f

cleandeps:
	cd submodules/glfw && git checkout . && git clean -dfx
	cd submodules/umka && git checkout . && git clean -dfx
