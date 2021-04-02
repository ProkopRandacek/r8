#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SDFGen.h"
#include "shapes.h"
#include "../settings.h"

#define FRAG_SDF_SIZE 4096

char* sdf;
int isGroup(int i) { return i >= MAX_SHAPE_NUM; }

extern Shape*      shapes[MAX_SHAPE_NUM];
extern ShapeGroup* groups[MAX_GROUP_NUM];
extern int groupNum;

const char* SDFNames[] = {
	"d2Cube",
	"d2Sphere",
	"d2Cylinder",
	"d2BoxFrame",
	"d2Torus",
	"d2CTorus",
	"d2Link",
	"d2Plane",
	"d2HexPrism",
	"d2TriPrism",
	"d2Capsule",
	"d2CCone",
	"d2Pyramid",
	"d2Triangle",
	"d2Quad"
};

const char* SDFArgs[] = {
	/* cube    */"pos, sP(_), sS(_)",
	/* sphere  */"pos, sP(_), sR(_)",
	/* cyinder */"pos, sP(_), sS(_), sR(_)",
	/*         */"",
	/* torus   */"pos, sP(_), sRV(_)",
	/*         */"",
	/*         */"",
	/*         */"",
	/*         */"",
	/*         */"",
	/*         */"",
	/* ccone   */"pos, sP(_), sS(_), sF(_, 9), sF(_, 10)",
	/*         */"",
	/*         */"",
	/*         */""
};

// for replacing the `_` in SDFArgs with the actuall shape number
char* strReplace(const char* org, char pre, char* post) {
	int i = 0;
	int postL = (int)strlen(post);
	char* buff = malloc(strlen(org) + strlen(post) + 1);
	memset(buff, 0, strlen(org) + strlen(post) + 1);
	while (1) {
		if      (org[i] == 0) break;
		else if (org[i] == pre) {
			strcat(buff, post);
			i += postL;
		} else {
			buff[i] = org[i];
			i++;
		}

	}
	return buff;
}

void makeShape(int i) {
	char* c = malloc(sizeof(char) * 128);
	memset(c, 0, sizeof(char) * 128);

	char str[5];
	sprintf(str, "%d", i);

	char* a = strReplace(SDFArgs[shapes[i]->type - 1], '_', str);

	sprintf(c, "map(sC(%d), %s(%s))", i, SDFNames[shapes[i]->type - 1], a);

	strcat(sdf, c);
	free(c);
	free(a);
}

//"map(vec4(0,1,1,0),d2Cube    (pos,vec3(0),vec3(0.9)))",

// recurively build the sdf from groups tree
void recr(int pos) {
	ShapeGroup* me = groups[pos - MAX_SHAPE_NUM];

	if      (me->op == NORMAL) { strcat(sdf, "minM("); }
	else if (me->op == CUT)    { strcat(sdf, "cutM("); }
	else if (me->op == MASK)   { strcat(sdf, "maxM("); }

	if (isGroup(me->a)) { recr(me->a); }
	else { makeShape(me->a); }
	strcat(sdf, ",");
	if (isGroup(me->b)) { recr(me->b); }
	else { makeShape(me->b); }
	strcat(sdf, ")");
}

char* genSDF() {
	sdf = malloc(sizeof(char) * FRAG_SDF_SIZE);
	memset(sdf, 0, FRAG_SDF_SIZE);
	recr(groupNum - 1 + MAX_SHAPE_NUM);

	return sdf;
}

