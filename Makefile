CFLAGS ?= -O3 -march=native -pipe

LDLIBS += -lm
WARNS  += -Wall -Wunused-parameter -Wshadow -Wundef -Wfloat-equal -Wunreachable-code -Wno-misleading-indentation

LDLIBS += $(shell pkg-config --libs   raylib libucw)
CFLAGS += $(shell pkg-config --cflags raylib libucw)

TARGET = r8

S = src

all: $(TARGET)

include src/Makefile

clean::
	rm -f $(TARGET)

clear: clean

install: $(TARGET)
	install -m 755 -D $(TARGET) $(DESTDIR)/usr/bin/r8

