.SUFFIXES :
.PHONY : all clean deepclean raylib docs docclean

TARGET = r8

VERSION     ?= v0.0.0
COMMIT_HASH ?= $(shell git rev-parse --short HEAD)

DEFS = -DR8_VERSION=\"$(VERSION)\" -DR8_COMMIT_HASH=\"$(COMMIT_HASH)\"

CFLAGS   += -O3 -pipe -std=c11   $(DEFS)
CXXFLAGS += -O3 -pipe -std=c++11 $(DEFS)
LDLIBS += -lm -ldl -lpthread

 WARNS ?= \
	-Wall -Wextra -Wunused-parameter -Wundef -Wunreachable-code \
	-Wno-misleading-indentation -Wenum-conversion -Wswitch-enum \
	-Wenum-compare -Wswitch -Wswitch-enum -Wno-free-nonheap-object \
	-Wunused-macros -Wdangling-else -Wstrict-prototypes \
	-Wmissing-field-initializers -Wno-multichar

FFLAGS = -fmerge-all-constants -funroll-loops

ifeq ($(word 2, $(shell $(CC) --version)), "(GCC)") # are we using gcc?
	FFLAGS += -fanalyzer # clang doesn't support -fanalyzer flag
endif

CFLAGS += $(WARNS) $(FFLAGS)

LIBRARIES =

all: $(TARGET)

.ONESHELL : docs
docs:
	f="docs/docmain.md"
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
include Makeraygui
include Makeumka
include Makeopenvr

include util/Makefile
include src/Makefile
#include Makecross

