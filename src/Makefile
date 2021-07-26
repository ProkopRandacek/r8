# included from ../Makefile
_:
	@echo run make from parent directory && false

DEPS = $(wildcard src/*.h)
SRCS = $(wildcard src/*.c)
SHDS = $(wildcard src/*.glsl)

OBJS = $(sort $(SRCS:.c=.o))
SHDH = $(addsuffix .h,$(SHDS))


clean::
	rm -f $(OBJS) $(SHDH)

%.o: %.c $(DEPS) $(SHDH)
	@echo CC $@
	@$(CC) $(WARNS) $(CFLAGS) -c $< -o $@

%.glsl.h: %.glsl
	@echo SHADER $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " > $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@

$(TARGET): $(SHDH) $(OBJS)
	@echo LD $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
