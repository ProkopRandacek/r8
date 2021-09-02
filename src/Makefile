# included from ../Makefile

DIRS = $(shell find src/ -type d)        # all folders
SRCS = $(shell find src/ -name \*.c)     # c sources
SHDS = $(shell find src/ -name \*.glsl)  # shader sources
ASTS = $(shell find src/ -name \*.asset) # asset sources
SHDH = $(addsuffix .h,$(SHDS))           # shader headers
ASTH = $(addsuffix .h,$(ASTS))           # asset headers
OBJS = $(sort $(SRCS:.c=.o))             # object files
DEPS = $(sort $(SRCS:.c=.d))             # makefile dependency files

# Include all source directories so I don't have to use relative paths in includes
# This way I can just include the filename and it will find it.
CFLAGS += $(addprefix -I,$(DIRS))

src/%.o: src/%.c $(SHDH) $(ASTH)
	@echo CC $@
	@$(CC) $(CFLAGS) -MM -MG -MF $(patsubst %.o,%.d,$@) -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

%.asset.h: %.asset $(BIEM)
	@echo EM $@
	@$(BIEM) $< $(notdir $<) $@

%.glsl.h: %.glsl
	@echo EM $@
	@echo "#ifndef $(notdir $(basename $^))_H" >  $@
	@echo "#define $(notdir $(basename $^))_H" >> $@
	@echo "const unsigned int $(notdir $(basename $^))_glsl_len = $(shell wc -c $^ | awk '{print $$1}');" >> $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " >> $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@

$(TARGET): $(LIBRARIES) $(OBJS)
	@echo LD $@
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

clean::
	$(RM) $(OBJS)
	$(RM) $(SHDH)
	$(RM) $(DEPS)
	$(RM) $(ASTH)

-include $(DEPS)
