#include <stdio.h>
#include <stdlib.h>
#include <microui.h>

#include "editor.h"

void editor_iter(mu_Context* ctx, Editor* e, Shape* pos);
void editor_draw_node(mu_Context* ctx, Editor* e, Shape* s);

void editor_tree(mu_Context* ctx, Editor* e) {
	if (mu_begin_window(ctx, "Scene tree", mu_rect(0, 450, 300, 450))) {
		Shape* root = e->scene->root;
		editor_iter(ctx, e, root);
		mu_end_window(ctx);
	}
}

void editor_iter(mu_Context* ctx, Editor* e, Shape* pos) {
	if (mu_begin_treenode(ctx, pos->name)) {
		editor_draw_node(ctx, e, pos);

		switch (pos->type) {
			case stPRIMITIVE:
				break;
			case stGROUP:
				editor_iter(ctx, e, pos->g.a);
				editor_iter(ctx, e, pos->g.b);
				break;
			case stWRAPPER:
				editor_iter(ctx, e, pos->w.shape);
				break;
		}
		mu_end_treenode(ctx);
	}
}

void editor_draw_node(mu_Context* ctx, Editor* e, Shape* s) {
	if (mu_button(ctx, "Selected"))
		e->selected_shape = s;
}
