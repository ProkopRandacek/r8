.PHONY : all clean deepclean raylib docs

CROSS_CC = x86_64-w64-mingw32-gcc
CROSS_FLAGS = -lm -lwinmm -Ldl -lopengl32 -lgdi32 -Wl,-Bstatic -lpthread $(WARNS)

TARGET = r8

CFLAGS += -O3 -pipe -std=gnu11 -g -DR8_DEBUG
LDLIBS += -lm -ldl -lpthread

override WARNS += \
	-Wall -Wextra -Wunused-parameter -Wshadow -Wundef -Wunreachable-code \
	-Wno-misleading-indentation -Wenum-conversion -Waggregate-return \
	-Wswitch-enum -Wenum-compare -Wswitch -Wswitch-enum \
	-Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn \
	-Walloc-zero -Wduplicated-branches -Wduplicated-cond \
	-Wno-free-nonheap-object -Wunused-macros -Wdangling-else \
	-Wstrict-prototypes -Wmissing-field-initializers -Wno-multichar

FFLAGS = -fmerge-all-constants -funroll-loops

ifeq ($(word 2, $(shell $(CC) --version)), "(GCC)") # are we using gcc?
	FFLAGS += -fanalyzer # clang doesn't support -fanalyzer flag
endif

CFLAGS += $(WARNS) $(FFLAGS)

LIBRARIES =

all: $(TARGET)

.ONESHELL : docs
docs:
	f="docmain.md"
	h="$(shell git rev-parse HEAD)"
	r="https://github.com/ProkopRandacek/r8"
	$(RM) $$f
	cp readme.md $$f
	echo -n "Generated at \`$(shell date)\` from [$${h:0:10}]($$r/tree/$$h)" >> $$f
	git diff --quiet || echo " (dirty)" >> $$f
	doxygen

run: $(TARGET)
	./r8

deepclean:: clean docclean

docclean:
	$(RM) docmain.md doxydocs/ -r

clean::
	$(RM) $(TARGET)

include Makeraylib

include Makeumka

include src/Makefile

