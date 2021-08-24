.PHONY : all clean deepclean libucw raylib

CFLAGS ?= -O3 -march=native -pipe -std=gnu11
LDLIBS += -lm -ldl -lpthread

override WARNS += \
	-Wall -Wextra -Wunused-parameter -Wshadow -Wundef -Wunreachable-code \
	-Wno-misleading-indentation -Wenum-conversion -Waggregate-return \
	-Wconversion -Wswitch-enum -Wenum-compare -Wswitch -Wswitch-enum \
	-Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn \
	-Walloc-zero -Wduplicated-branches -Wduplicated-cond \
	-Wno-free-nonheap-object -Wunused-macros -Wdangling-else \
	-Wstrict-prototypes -Wmissing-field-initializers -Wno-multichar

FFLAGS = -fmerge-all-constants -funroll-loops

ifeq ($(word 2, $(shell $(CC) --version)), "(GCC)") # are we using gcc?
	FFLAGS += -fanalyzer # clang doesn't support -fanalyzer flag
endif

CFLAGS += $(WARNS) $(FFLAGS)

TARGET = r8

all: $(TARGET)

docs:
	doxygen

run: $(TARGET)
	./r8

include Makelibucw

include Makeraylib

#include Makeraygui

include src/Makefile

deepclean:: clean
	$(RM) doxydocs -r

clean::
	$(RM) $(TARGET)

