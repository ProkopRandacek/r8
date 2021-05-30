#include "shaderGen.h"

#include "glslRead.h"

#define FRAG_FULL_SIZE     65536 // 2^16
#define FRAG_SDF_SIZE       8192 // 2^13
#define FRAG_END_SIZE       8192 // 2^12
#define FRAG_MAP_SIZE       8192 // 2^12
#define FRAG_HEADER_SIZE    8192 // 2^11
#define FRAG_SETTINGS_SIZE  8192 // 2^10
#define FRAG_VERSION_SIZE   8192 // 2^4

#define VERT_SIZE 128

#define GLSL_VERSION 330

char* sdf; // TODO do this without global variables because thats kinda dumb
char* sdfd;

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

int isGroup(int i) { return i >= MAX_SHAPE_NUM; }

// for replacing the `_` in SDFArgs with the actual shape number
char* strReplace(const char* org, char pre, char* post) {
	int i = 0;
	int postL = (int)strlen(post);
	char* buff = calloc(1, strlen(org) + strlen(post) + 1);
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

void makeShape(Scene* s, int i) {
	char* c = calloc(1, 128);

	char str[5];
	sprintf(str, "%d", i);

	char* a = strReplace(SDFArgs[s->shapes[i]->type - 1], '_', str);

	sprintf(c, "map(sC(%d), %s(%s))", i, SDFNames[s->shapes[i]->type - 1], a);

	strcat(sdf, c);
	free(c);
	free(a);
}

void makeShapeD(Scene* s, int i) {
	char* c = calloc(1, 128);

	char str[5];
	sprintf(str, "%d", i);

	char* a = strReplace(SDFArgs[s->shapes[i]->type - 1], '_', str);

	sprintf(c, "%s(%s)", SDFNames[s->shapes[i]->type - 1], a);

	strcat(sdfd, c);
	free(c);
	free(a);
}

// recurively build the sdf from groups tree
void makeSDF(Scene* s, int pos) {
	ShapeGroup* me = s->groups[pos - MAX_SHAPE_NUM];

	if      (me->op == NORMAL)  { strcat(sdf, "minM("); }
	else if (me->op == CUT)     { strcat(sdf, "cutM("); }
	else if (me->op == MASK)    { strcat(sdf, "maxM("); }
	else if (me->op == AVERAGE) {
		strcat(sdf, "avgM(");
		char c[5];
		sprintf(&c, "%.2f", me->k);
		strcat(sdf, c);
		strcat(sdf, ", ");
	}

	if (isGroup(me->a)) { makeSDF(s, me->a); }
	else { makeShape(s, me->a); }
	strcat(sdf, ", ");
	if (isGroup(me->b)) { makeSDF(s, me->b); }
	else { makeShape(s, me->b); }
	strcat(sdf, ")");
}

void makeSDFD(Scene* s, int pos) {
	ShapeGroup* me = s->groups[pos - MAX_SHAPE_NUM];

	if      (me->op == NORMAL)  { strcat(sdfd, "min("); }
	else if (me->op == CUT)     { strcat(sdfd, "max("); }
	else if (me->op == MASK)    { strcat(sdfd, "max("); }
	else if (me->op == AVERAGE) { strcat(sdfd, "mix("); }

	if (isGroup(me->a)) { makeSDFD(s, me->a); }
	else { makeShapeD(s, me->a); }
	strcat(sdfd, ", ");

	if (me->op == CUT) { strcat(sdfd, "-"); }

	if (isGroup(me->b)) { makeSDFD(s, me->b); }
	else { makeShapeD(s, me->b); }

	if (me->op == AVERAGE) {
		strcat(sdfd, ", ");
		char c[5];
		sprintf(&c, "%.2f", me->k);
		strcat(sdfd, c);
	}

	strcat(sdfd, ")");
}

char* genSDF(Scene* s) {
	sdf = calloc(1, FRAG_SDF_SIZE);
	makeSDF(s, s->groupNum - 1 + MAX_SHAPE_NUM);

	return sdf;
}

char* genSDFD(Scene* s) {
	sdfd = calloc(1, FRAG_SDF_SIZE);
	makeSDFD(s, s->groupNum - 1 + MAX_SHAPE_NUM);

	return sdfd;
}

char* createVertSource() {
	char* c = calloc(1, VERT_SIZE);
	char* ver = createVersionSource();
	char* vertSource = readGlsl("vertSource");
	strcat(c, ver);
	strcat(c, vertSource);
	free(ver);
	free(vertSource);
	printf("\n\n%s\n\n", c);
	return c;
}

char* createVersionSource() {
	char* c = calloc(1, FRAG_VERSION_SIZE);
	char* version = "#version ";

	sprintf(c, "%s%d\n", version, GLSL_VERSION);

	return c;
}

char* createSettingsSource(Scene* s) {
	char* c = calloc(1, FRAG_SETTINGS_SIZE);
	int   shapeSize = SHAPE_SIZE;
	int   groupSize = GROUP_SIZE;
	float backStep  = s->collisionThreshold * s->backStepK;

	char                maxShapeNum_[10] = {0, 0, 0, 0, 0, 0};
	char                maxGroupNum_[10] = {0, 0, 0, 0, 0, 0};
	char                  shapeSize_[10] = {0, 0, 0, 0, 0, 0};
	char                  groupSize_[10] = {0, 0, 0, 0, 0, 0};
	char                    epsilon_[10] = {0, 0, 0, 0, 0, 0};
	char                   STEPSNUM_[10] = {0, 0, 0, 0, 0, 0};
	char        COLLISION_THRESHOLD_[10] = {0, 0, 0, 0, 0, 0};
	char SHADOW_COLLISION_THRESHOLD_[10] = {0, 0, 0, 0, 0, 0};
	char                  BACK_STEP_[10] = {0, 0, 0, 0, 0, 0};
	char             MAX_TRACE_DIST_[10] = {0, 0, 0, 0, 0, 0};
	char                    BOUNCES_[10] = {0, 0, 0, 0, 0, 0};
	char                   SUN_SIZE_[10] = {0, 0, 0, 0, 0, 0};

	sprintf(               maxShapeNum_, "%d"  , s->maxShapeNum             );
	sprintf(               maxGroupNum_, "%d"  , s->maxGroupNum             );
	sprintf(                 shapeSize_, "%d"  , shapeSize                  );
	sprintf(                 groupSize_, "%d"  , groupSize                  );
	sprintf(                   epsilon_, "%.2f", s->epsilon                 );
	sprintf(                  STEPSNUM_, "%d"  , s->stepsNum                );
	sprintf(       COLLISION_THRESHOLD_, "%.3f", s->collisionThreshold      );
	sprintf(SHADOW_COLLISION_THRESHOLD_, "%.3f", s->shadowCollisionThreshold);
	sprintf(                 BACK_STEP_, "%.3f", backStep                   );
	sprintf(            MAX_TRACE_DIST_, "%.3f", s->maxTraceDist            );
	sprintf(                   BOUNCES_, "%d"  , s->bounces                 );
	sprintf(                  SUN_SIZE_, "%.3f", s->sunSize                 );

	strcat(c, "// SETTINGS START\n");
	strcat(c,   "#define maxShapeNum "               ); strcat(c,                maxShapeNum_);
	strcat(c, "\n#define maxGroupNum "               ); strcat(c,                maxGroupNum_);
	strcat(c, "\n#define shapeSize "                 ); strcat(c,                  shapeSize_);
	strcat(c, "\n#define groupSize "                 ); strcat(c,                  groupSize_);
	strcat(c, "\n#define epsilon "                   ); strcat(c,                    epsilon_);
	strcat(c, "\n#define STEPSNUM "                  ); strcat(c,                   STEPSNUM_);
	strcat(c, "\n#define COLLISION_THRESHOLD "       ); strcat(c,        COLLISION_THRESHOLD_);
	strcat(c, "\n#define SHADOW_COLLISION_THRESHOLD "); strcat(c, SHADOW_COLLISION_THRESHOLD_);
	strcat(c, "\n#define BACK_STEP "                 ); strcat(c,                  BACK_STEP_);
	strcat(c, "\n#define MAX_TRACE_DIST "            ); strcat(c,             MAX_TRACE_DIST_);
	strcat(c, "\n#define BOUNCES "                   ); strcat(c,                    BOUNCES_);
	strcat(c, "\n#define SUN_SIZE "                  ); strcat(c,                   SUN_SIZE_);
	strcat(c, "\n// SETTINGS END\n\n");

	return c;
}

char* createHeaderSource() {
	char* c = calloc(1, FRAG_HEADER_SIZE);
	char* s;

	strcat(c, "// HEADER START\n");
	s = readGlsl("defines" ); strcat(c, s); free(s);
	s = readGlsl("uniforms"); strcat(c, s); free(s);
	s = readGlsl("const"   ); strcat(c, s); free(s);
	s = readGlsl("out"     ); strcat(c, s); free(s);
	s = readGlsl("structs" ); strcat(c, s); free(s);
	strcat(c, "// HEADER END\n\n");

	return c;
}

char* createSDFsSource(short shapesMask) {
	char* c = calloc(1, FRAG_SDF_SIZE);

	//shapesMask = shapesMask | SPHERE_MASK;

	strcat(c, "// SDFS START\n");
	if (shapesMask & SPHERE_MASK  ) { char* s = readSDF("sphere"  ); strcat(c, s); free(s); }
	if (shapesMask & CUBE_MASK    ) { char* s = readSDF("cube"    ); strcat(c, s); free(s); }
	if (shapesMask & CYLINDER_MASK) { char* s = readSDF("cylinder"); strcat(c, s); free(s); }
	if (shapesMask & TORUS_MASK   ) { char* s = readSDF("torus"   ); strcat(c, s); free(s); }
	if (shapesMask & CCONE_MASK   ) { char* s = readSDF("ccone"   ); strcat(c, s); free(s); }
	if (shapesMask & BOXFRAME_MASK) { char* s = readSDF("boxFrame"); strcat(c, s); free(s); }
	if (shapesMask & CTORUS_MASK  ) { char* s = readSDF("ctorus"  ); strcat(c, s); free(s); }
	if (shapesMask & LINK_MASK    ) { char* s = readSDF("link"    ); strcat(c, s); free(s); }
	if (shapesMask & PLANE_MASK   ) { char* s = readSDF("plane"   ); strcat(c, s); free(s); }
	if (shapesMask & HEXPRISM_MASK) { char* s = readSDF("hexPrism"); strcat(c, s); free(s); }
	if (shapesMask & TRIPRISM_MASK) { char* s = readSDF("triPrism"); strcat(c, s); free(s); }
	if (shapesMask & CAPSULE_MASK ) { char* s = readSDF("capsule" ); strcat(c, s); free(s); }
	if (shapesMask & PYRAMID_MASK ) { char* s = readSDF("pyramid" ); strcat(c, s); free(s); }
	if (shapesMask & TRIANGLE_MASK) { char* s = readSDF("triangle"); strcat(c, s); free(s); }
	if (shapesMask & QUAD_MASK    ) { char* s = readSDF("quad"    ); strcat(c, s); free(s); }
	strcat(c, "// SDFS END\n\n");

	return c;
}

char* createMapWorldSource(Scene* s) {
	char* c = calloc(1, FRAG_MAP_SIZE);
	char* SDF  = genSDF (s);
	char* SDFD = genSDFD(s);
	char* x;

	strcat(c, "// MAP WORLD START\n");
	x = readGlsl("combineFuncs" ); strcat(c, x); free(x);
	x = readGlsl("mapFuncStart" ); strcat(c, x); free(x);
	strcat(c, SDF);
	x = readGlsl("mapFuncMid" ); strcat(c, x); free(x);
	strcat(c, SDFD);
	x = readGlsl("mapFuncEnd" ); strcat(c, x); free(x);
	strcat(c, "// MAP WORLD END\n\n");

	free(sdf);
	free(sdfd);

	return c;
}

char* createEndSource() {
	char* c = calloc(1, FRAG_END_SIZE);
	char* s;

	strcat(c, "// END START\n");
	s = readGlsl("normalFunc"        ); strcat(c, s); free(s);
	s = readGlsl("intersectionFunc"  ); strcat(c, s); free(s);
	s = readGlsl("rayMarchFunc"      ); strcat(c, s); free(s);
	s = readGlsl("rayMarchShadowFunc"); strcat(c, s); free(s);
	s = readGlsl("mainFunc"          ); strcat(c, s); free(s);
	strcat(c, "// END END\n\n");

	return c;
}

char* createFragSource(Scene* s) {
	char* c = calloc(1, FRAG_FULL_SIZE);
	char* vers = createVersionSource();
	char* settings = createSettingsSource(s);
	char* header = createHeaderSource();
	char* SDFs = createSDFsSource(s->shapeMask);
	char* mapWorld = createMapWorldSource(s);
	char* end = createEndSource();

	strcat(c, vers);
	strcat(c, settings);
	strcat(c, header);
	strcat(c, SDFs);
	strcat(c, mapWorld);
	strcat(c, end);

	free(vers);
	free(settings);
	free(header);
	free(SDFs);
	free(mapWorld);
	free(end);

	//printf("%s", c);
	FILE* fp = fopen("./fragShader.glsl", "w+");
	fputs(c, fp);
	fclose(fp);

	return c;
}

