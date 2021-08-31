# included from ../Makefile
_:
	@echo run make from repository root && false

DIRS = $(shell find src/ -type d)            # all folders
SRCS = $(shell find src/ -name \*.c -print)  # c sources
SHDS = $(shell find src/ -name \*.glsl)      # shader sources
OBJS = $(sort $(SRCS:.c=.o))                 # object files
SHDH = $(addsuffix .h,$(SHDS))               # shader headers
DEPS = $(OBJS:.o=.d)

# Include all source directories so I don't have to use relative paths in includes
# This way I can just include the filename and it will find it.
CFLAGS += $(addprefix -I,$(DIRS))

-include $(DEPS)

%.o: %.c $(SHDH)
	@echo CC $@
	@$(CC) $(CFLAGS) -MM -MG -MF $(patsubst %.o,%.d,$@) -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

%.glsl.h: %.glsl
	@echo EM $@
	@echo "#ifndef $(notdir $(basename $^))_H" >  $@
	@echo "#define $(notdir $(basename $^))_H" >> $@
	@echo "const unsigned int $(notdir $(basename $^))_glsl_len = $(shell wc -c $^ | awk '{print $$1}');" >> $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " >> $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@

$(TARGET): $(LIBRARIES) $(SHDH) $(OBJS)
	@echo LD $@
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

clean::
	$(RM) $(OBJS)
	$(RM) $(SHDH)
	$(RM) $(DEPS)

