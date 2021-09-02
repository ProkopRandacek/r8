#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include "scene.h"
#include "alloc.h"
#include "log.h"

static int s_pos = 0, g_pos = 0;
static char* sdf;

void scene_sdf_gen(Shape *pos) {
	char *c;
	switch (pos->type) {
		case stPRIMITIVE:
			switch (pos->p.type) {
				case ptSPHERE:
					asprintf(&c, "clrd(sC(%d),d2Sphere(pos,sP(%d),sR1(%d)))", s_pos, s_pos, s_pos);
					break;
				case ptCUBE:
					asprintf(&c, "clrd(sC(%d),d2Cube(pos,sP(%d),sS(%d)))", s_pos, s_pos, s_pos);
					break;
				case ptTORUS:
				case ptCTORUS:
				case ptCYL:
				case ptCCONE:
					die("primitive type %d not implemented", pos->p.type);
					break;
			}
			s_pos++;
			strcat(sdf, c);
			xfree(c);
			break;
		case stGROUP:
			switch (pos->g.type) {
				case gtUNION:       asprintf(&c, "u(");           break;
				case gtDIFF:        asprintf(&c, "d(");           break;
				case gtINTERS:      asprintf(&c, "i(");           break;
				case gtBLEND:       asprintf(&c, "b(gK(%d),", g_pos); break;
				case gtAVERAGE:     asprintf(&c, "a(gK(%d),", g_pos); break;
						    //case gtAPPROXIMATE: asprintf(&c, "x(");           break;
				case gtAPPROXIMATE: die("group type approximate not implemented");
			}
			g_pos++;
			strcat(sdf, c);
			xfree(c);

			scene_sdf_gen(pos->g.a);
			strcat(sdf, ",");
			scene_sdf_gen(pos->g.b);
			strcat(sdf, ")");
			break;
		case stWRAPPER:
			die("wrappers are not implemented");
			break;
	}
}

char* scene_create_sdf(Scene *s) {
	sdf = xmalloc_zero(8192);

	scene_sdf_gen(s->root);

	return xrealloc(sdf, strlen(sdf) + 1);
}

