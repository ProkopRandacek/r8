// This file holds the raygui implementation
// Other files that want to use raygui include the `_raygui.h` file
// which only includes raygui as a header
#include "alloc.h"
// use our allocators
#define RAYGUI_MALLOC(sz)   xmalloc(sz)
#define RAYGUI_CALLOC(n,sz) xmalloc_zero((n)*(sz))
#define RAYGUI_FREE(p)      xfree(p)
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
//#define RAYGUI_SUPPORT_CUSTOM_RICONS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#include <raygui.h>
#pragma GCC diagnostic pop

