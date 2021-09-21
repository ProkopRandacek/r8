# included from ../Makefile
_::
	@echo run make from repo root

DIRS = $(shell find src/ -type d)                # all folders
SRCS = $(shell find src/ -type f -name \*.c)     # c sources
ASTS = $(shell find src/ -type f -name \*.asset) # asset sources
ASTH = $(addsuffix .h,$(ASTS))                   # asset headers
OBJS = $(sort $(SRCS:src/%.c=obj/%.o))           # object files
DEPS = $(sort $(SRCS:src/%.c=obj/%.d))           # makefile dependency files

# Include all source directories so I don't have to use relative paths in includes
CFLAGS += $(addprefix -I,$(DIRS))

$(ASTH): src/%.asset.h: src/%.asset $(BIEM)
	@mkdir -p -- $(dir $@)
	@echo -e "EM\t$(notdir $@)"
	@$(BIEM) $< $(notdir $<) $@

$(OBJS): obj/%.o: src/%.c $(ASTH)
	@mkdir -p -- $(dir $@)
	@echo -e "CC\t$(notdir $@)"
	@$(CC) $(CFLAGS) -MD -o $@ -c $<

$(TARGET): $(LIBRARIES) $(OBJS)
	@mkdir -p -- $(dir $@)
	@echo -e "LD\t$(notdir $@)"
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LDFLAGS)

clean::
	$(RM) -- $(SHDH)
	$(RM) -- $(ASTH)
	$(RM) -r -- obj/

-include $(DEPS)

