all: runtree programs datafiles

# Include configuration
s=.
-include obj/config.mk
obj/config.mk:
	@echo "You need to run configure first." && false

# Do not show strange errors if the BUILDSYS is not set
# (it happens if noone called configure as reported above)
ifdef BUILDSYS

# We will use the libucw build system
include $(BUILDSYS)/Maketop

# Add the detected flags to all the global flags
CFLAGS+=$(LIBUCW_CFLAGS) $(RAYLIB_CCFLAGS)
LIBS+=$(LIBUCW_LIBS) $(RAYLIB_LIBS)

DIRS+=.

R8_MODS=r8 log
R8_MOD_PATHS=$(addprefix $(o)/,$(R8_MODS))

# Programs we want to compile
PROGS+=$(o)/r8

# And how they are created
$(o)/r8: $(o)/r8.o $(addsuffix .o,$(R8_MOD_PATHS)) $(LIBUCW)

include $(s)/assets/Makefile

# And finally the default rules of the build system
include $(BUILDSYS)/Makebottom

endif
