.PHONY : all clean deepclean libucw raylib

CFLAGS ?= -O3 -march=native -pipe -std=gnu17

LDLIBS += -lm -ldl -lpthread
override WARNS  += -Wall -Wunused-parameter -Wshadow -Wundef -Wunreachable-code -Wno-misleading-indentation

CFLAGS += $(WARNS)

TARGET = r8

all: $(TARGET)

run: $(TARGET)
	./r8

include Makelibucw
	
include Makeraylib

#include Makeraygui

include src/Makefile

deepclean:: clean

clean::
	$(RM) $(TARGET)

