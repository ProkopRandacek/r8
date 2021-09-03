#include "_raygui.h"
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

