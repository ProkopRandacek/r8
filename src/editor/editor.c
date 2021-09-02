#define RAYGUI_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#include <raygui.h>
#pragma GCC diagnostic pop

#include <raylib.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

Editor *editor_new(Scene *s) {
	Editor *e = xmalloc(sizeof(Editor));
	e->scene = s;
	return e;
}

void editor_draw(Editor *e) {
	editor_draw_scene_tree(e->scene);
}

