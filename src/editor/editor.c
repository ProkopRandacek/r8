#include <raylib.h>
#include <rlgl.h>
#include <stdio.h>

#include "editor.h"
#include "alloc.h"
#include "log.h"

/**
 * Scene updates are done at the end of a frame if anybody set this flag.
 *
 * (Shape created, deleted, edited, ...)
 */
bool update_scene;

extern Font def_font; // from r8.c

Editor *editor_new(Scene *s) {
	Editor *e = xmalloc_zero(sizeof(Editor));
	e->scene = s;
	e->unit = 20.0f;

	e->editor_layout = (Vector2){ GetScreenWidth() / 4.0f, GetScreenHeight() / 2.0f };

	return e;
}

void editor_draw(mu_Context* ctx, Editor *e) {
	mu_begin(ctx);


	if (e->selected_shape != NULL)
		editor_properties(ctx, e);

	e->selected_shape = e->scene->root->g.b;

	mu_end(ctx);

	mu_Command *cmd = NULL;
	while (mu_next_command(ctx, &cmd)) {
		switch (cmd->type) {
			case MU_COMMAND_TEXT:
				{
					Vector2 pos = (Vector2){cmd->text.pos.x, cmd->text.pos.y};
					Color clr = (Color){cmd->text.color.r, cmd->text.color.g, cmd->text.color.b, cmd->text.color.a};
					DrawTextEx(def_font, cmd->text.str, pos, 20, 0, clr);
					break;
				}
			case MU_COMMAND_RECT:
				{
					Rectangle r = (Rectangle){cmd->rect.rect.x, cmd->rect.rect.y, cmd->rect.rect.w, cmd->rect.rect.h};
					Color clr = (Color){cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a};
					DrawRectangleRec(r, clr);
					break;
				}
			case MU_COMMAND_ICON: //r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
				{
					Rectangle r = (Rectangle){cmd->icon.rect.x, cmd->icon.rect.y, cmd->icon.rect.w, cmd->icon.rect.h};
					Color clr = (Color){cmd->icon.color.r, cmd->icon.color.g, cmd->icon.color.b, cmd->icon.color.a};
					DrawRectangleRec(r, clr);
					break;
				}
			case MU_COMMAND_CLIP:
				{
					// Should I end the scissor mode somewhere?
					BeginScissorMode(cmd->clip.rect.x, cmd->clip.rect.y, cmd->clip.rect.w, cmd->clip.rect.h);
					break;
				}
		}
	}
}

void editor_destroy(Editor *e) {
	xfree(e);
}

