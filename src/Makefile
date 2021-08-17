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
	@echo "#ifndef $(notdir $(basename $^))_H" >  $@
	@echo "#define $(notdir $(basename $^))_H" >> $@
	@echo "const unsigned int $(notdir $(basename $^))_glsl_len = $(shell wc -c $^ | awk '{print $$1}');" >> $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " >> $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@

$(TARGET): $(SHDH) $(OBJS) $(LIBUCW_LIBRARY) $(RAYLIB_LIBRARY)
	@echo LD $@
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

