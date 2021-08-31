#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Waggregate-return"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#include <raylib.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"
#include "stack.h"

Editor *editor_new(Scene *s) {
	Editor *e = xmalloc(sizeof(Editor));
	e->scene = s;
	return e;
}

void editor_draw(Editor *e) {
	// draw the background
	Rectangle scene_tree_box = (Rectangle){ 0.0f, 0.0f, GetScreenWidth() / 4.0f, GetScreenHeight() / 1.0f };
	DrawRectangleRec(scene_tree_box, (Color){ 200, 200, 200, 128 });

	int x = 0, y = 0, p = 0;
	Stack *s = stack_new();
	stack_push(s, e->scene->root);
	while (s->top != 0) {
		Shape *pos = stack_pop(s);
		Rectangle text_box = (Rectangle){ x * 20.0f + 20.0f, y * 20.0f, 200.0f, 200.0f};
		y++;
		switch (pos->type) {
			case stPRIMITIVE:
				GuiLabel(text_box, "prim");
				if (--p == 0) x--;
				break;
			case stGROUP:
				GuiLabel(text_box, "group");
				x++;
				p++;
				stack_push(s, pos->g.b);
				stack_push(s, pos->g.a);
				break;
			case stWRAPPER:
				GuiLabel(text_box, "wrap");
				p++;
				stack_push(s, pos->w.shape);
				break;
			default:
				die("invalid shape type %d", pos->type);
		}
	}
	stack_destroy(s);
}

