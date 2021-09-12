#include <stdio.h>
#include <microui.h>

#include "editor.h"

void editor_properties(mu_Context *ctx, Editor* e) {
	/* do window */
	if (mu_begin_window(ctx, "Shape Properties", mu_rect(0, 0, 300, 450))) {
		mu_Container *win = mu_get_current_container(ctx);
		win->rect.w = mu_max(win->rect.w, 240);
		win->rect.h = mu_max(win->rect.h, 300);

		/* window info */
		if (mu_header(ctx, "Shape base")) {
			mu_label(ctx, "Color"); {
				float *clr = e->selected_shape->p.d + 3;
				mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 69);
				mu_layout_begin_column(ctx);
				mu_layout_row(ctx, 2, (int[]) { 60, -1 }, 0);
				mu_label(ctx, "Red:");   mu_slider(ctx, &clr[0], 0, 1);
				mu_label(ctx, "Green:"); mu_slider(ctx, &clr[1], 0, 1);
				mu_label(ctx, "Blue:");  mu_slider(ctx, &clr[2], 0, 1);
				mu_layout_end_column(ctx);
				mu_Rect r = mu_layout_next(ctx);
				mu_draw_rect(ctx, r, mu_color(clr[0] * 255, clr[1] * 255, clr[2] * 255, 255));
				char buf[32];
				sprintf(buf, "#%02X%02X%02X", (int)(clr[0] * 255), (int)(clr[1] * 255), (int)(clr[2] * 255));
				mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
			}

			mu_label(ctx, "Position"); {
				mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 69);
				mu_layout_begin_column(ctx);
				static char buf[16];
				mu_layout_row(ctx, 2, (int[]) { 60, -1 }, 0);
				mu_label(ctx, "X"); mu_textbox(ctx, buf, 16);
				mu_layout_end_column(ctx);
			}
		}

		mu_end_window(ctx);
	}
}
