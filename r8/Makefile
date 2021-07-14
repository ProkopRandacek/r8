# Example Makefile for a stand-alone program using the libucw build system
# (c) 2007 Martin Mares <mj@ucw.cz>
# (c) 2008 Michal Vaner <vorner@ucw.cz>

# The default target
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

# Programs we want to compile
PROGS+=$(o)/r8 

# And how they are created
$(o)/r8: $(o)/r8.o $(LIBUCW)

include $(s)/assets/Makefile

# And finally the default rules of the build system
include $(BUILDSYS)/Makebottom

endif
