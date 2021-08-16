# included from ../Makefile
_:
	@echo run make from repository root && false

SRCS = $(wildcard src/*.c)     # c sources
SHDS = $(wildcard src/*.glsl)  # shader sources
OBJS = $(sort $(SRCS:.c=.o))   # object files
SHDH = $(addsuffix .h,$(SHDS)) # shader headers
DEPS = $(OBJS:.o=.d)

clean::
	$(RM) $(OBJS)
	$(RM) $(SHDH)
	$(RM) $(DEPS)


%.o: %.c
	@echo CC $@
	@$(CC) $(CFLAGS) -MM -MG -MF $(patsubst %.o,%.d,$@) -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

-include $(DEPS)

%.glsl.h: %.glsl
	@echo SD $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " > $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@

$(TARGET): $(SHDH) $(OBJS)
	@echo LD $@
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $@

