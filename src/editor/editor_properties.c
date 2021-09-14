#include <stdio.h>
#include <stdlib.h>
#include <microui.h>

#include "editor.h"

void editor_properties(mu_Context *ctx, Editor* e) {
	static Shape* last_shape = NULL;
	static bool shape_changed = false;
	if (e->selected_shape != last_shape) {
		shape_changed = true;
		last_shape = e->selected_shape;
	}

	/* do window */
	if (mu_begin_window(ctx, "Shape Properties", mu_rect(0, 0, 300, 450))) {
		mu_Container *win = mu_get_current_container(ctx);
		win->rect.w = mu_max(win->rect.w, 240);
		win->rect.h = mu_max(win->rect.h, 300);

		/* window info */
		if (mu_header(ctx, "Shape base")) {
			if (e->selected_shape->type == stPRIMITIVE) {
				mu_label(ctx, "Color"); {
					float *clr = e->selected_shape->p.d + 3; // the 4 color floats in data array
					mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 70);
					mu_layout_begin_column(ctx);
					mu_layout_row(ctx, 2, (int[]) { 60, -1 }, 0);
					int status = 0;
					mu_label(ctx, "Red:");   status |= mu_slider(ctx, &clr[0], 0, 1);
					mu_label(ctx, "Green:"); status |= mu_slider(ctx, &clr[1], 0, 1);
					mu_label(ctx, "Blue:");  status |= mu_slider(ctx, &clr[2], 0, 1);
					mu_label(ctx, "Alpha:"); status |= mu_slider(ctx, &clr[3], 0, 1);
					mu_layout_end_column(ctx);

					if (status & MU_RES_CHANGE) {
						e->selected_shape->p.d[3] = clr[0];
						e->selected_shape->p.d[4] = clr[1];
						e->selected_shape->p.d[5] = clr[2];
						e->selected_shape->p.d[6] = clr[3];
						e->scene->primt_changed = true;
					}

					mu_Rect r = mu_layout_next(ctx);
					mu_draw_rect(ctx, r, mu_color(clr[0] * 255, clr[1] * 255, clr[2] * 255, 255));
					char buf[32];
					sprintf(buf, "#%02x%02x%02x", (int)(clr[0] * 255), (int)(clr[1] * 255), (int)(clr[2] * 255));
					mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
				}

				mu_label(ctx, "Position"); {
					mu_layout_row(ctx, 1, (int[]) { -78, -1 }, 70);
					mu_layout_begin_column(ctx);
					static char x_buf[16],y_buf[16],z_buf[16];
					if (shape_changed) {
						sprintf(x_buf, "%.2f", e->selected_shape->p.d[0]);
						sprintf(y_buf, "%.2f", e->selected_shape->p.d[1]);
						sprintf(z_buf, "%.2f", e->selected_shape->p.d[2]);
						shape_changed = false;
					}
					int status = 0;
					mu_layout_row(ctx, 2, (int[]) { 60, -1 }, 0);
					mu_label(ctx, "X"); status |= mu_textbox(ctx, x_buf, 16);
					mu_label(ctx, "Y"); status |= mu_textbox(ctx, y_buf, 16);
					mu_label(ctx, "Z"); status |= mu_textbox(ctx, z_buf, 16);
					mu_layout_end_column(ctx);

					if (status & MU_RES_CHANGE) {
						float new_x = strtof(x_buf, NULL);
						float new_y = strtof(y_buf, NULL);
						float new_z = strtof(z_buf, NULL);
						e->selected_shape->p.d[0] = new_x;
						e->selected_shape->p.d[1] = new_y;
						e->selected_shape->p.d[2] = new_z;
						e->scene->primt_changed = true;
					}
				}
			} else if (e->selected_shape->type == stGROUP) {
				mu_label(ctx, "GROUP PLACEHOLDER");
			}

		}

		mu_end_window(ctx);
	}
}
