.PHONY : all clean deepclean libucw raylib

CFLAGS ?= -O3 -march=native -pipe

LDLIBS += -lm -ldl -lpthread
override WARNS  += -Wall -Wunused-parameter -Wshadow -Wundef -Wunreachable-code -Wno-misleading-indentation

CFLAGS += $(WARNS)

TARGET = r8

all: $(TARGET)

include Makelibucw
	
include Makeraylib

#include Makeraygui

include src/Makefile

deepclean:: clean

clean::
	$(RM) $(TARGET)

