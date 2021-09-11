# included from ../Makefile
_::
	@echo run make from repo root

S ?= .

DIRS = $(shell find $(S)/ -type d)        # all folders
SRCS = $(shell find $(S)/ -name \*.c)     # c sources
SHDS = $(shell find $(S)/ -name \*.glsl)  # shader sources
ASTS = $(shell find $(S)/ -name \*.asset) # asset sources
SHDH = $(addsuffix .h,$(SHDS))            # shader headers
ASTH = $(addsuffix .h,$(ASTS))            # asset headers
OBJS = $(sort $(SRCS:$(S)/%.c=obj/%.o))   # object files
DEPS = $(sort $(SRCS:$(S)/%.c=obj/%.d))   # makefile dependency files

# Include all source directories so I don't have to use relative paths in includes
CFLAGS += $(addprefix -I,$(DIRS))

$(ASTH): %.asset.h: %.asset $(BIEM)
	@mkdir -p -- $(dir $@)
	@echo EM $@
	@$(BIEM) $< $(notdir $<) $@

$(SHDH): %.glsl.h: %.glsl
	@mkdir -p -- $(dir $@)
	@echo EM $@
	@echo "#ifndef $(notdir $(basename $^))_H" >  $@
	@echo "#define $(notdir $(basename $^))_H" >> $@
	@echo "const unsigned int $(notdir $(basename $^))_glsl_len = $(shell wc -c $^ | awk '{print $$1}');" >> $@
	@echo -n "const char* $(notdir $(basename $^))_glsl = " >> $@
	@sed 's/^/"/; s/$$/\\n"/' $< >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@

$(OBJS): obj/%.o: $(S)/%.c $(ASTH) $(SHDH)
	@mkdir -p -- $(dir $@)
	@echo CC $@
	@$(CC) $(CFLAGS) -MM -MG -MF $(patsubst %.o,%.d,$@) -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(LIBRARIES) $(OBJS)
	@mkdir -p -- $(dir $@)
	@echo LD $@
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

clean::
	$(RM) $(OBJS)
	$(RM) $(SHDH)
	$(RM) $(DEPS)
	$(RM) $(ASTH)

-include $(DEPS)
