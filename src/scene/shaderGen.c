#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "head.h"
#include "shaderGen.h"
#include "SDFs.h"
#include "rayMarch.h"
#include "general.h"
#include "../settings.h"

#define FRAG_FULL_SIZE     65536 // 2^16
#define FRAG_END_SIZE       4096 // 2^12
#define FRAG_SDF_SIZE       4096 // 2^12
#define FRAG_MAP_SIZE       4096 // 2^12
#define FRAG_HEADER_SIZE    2048 // 2^11
#define FRAG_SETTINGS_SIZE  1024 // 2^10
#define FRAG_VERSION_SIZE     16 // 2^4

#define VERT_SIZE 128

#define GLSL_VERSION 330

char* createVertSource() {
	char* c = malloc(sizeof(char) * VERT_SIZE);
	memset(c, 0, VERT_SIZE);
	char* ver = createVersionSource();
	strcat(c, ver);
	strcat(c, vertSource);
	free(ver);
	return c;
}

char* createVersionSource() {
	char* c = malloc(sizeof(char) * FRAG_VERSION_SIZE);
	memset(c, 0, FRAG_VERSION_SIZE);

	sprintf(c, "%s%d\n", version, GLSL_VERSION);

	return c;
}
// 10 10 0.02 256 0.001, 0.001, 100.0f
char* createSettingsSource(Scene s) {
	char* c = malloc(sizeof(char) * FRAG_SETTINGS_SIZE);
	memset(c, 0, FRAG_SETTINGS_SIZE);
	int   shapeSize = SHAPE_SIZE;
	int   groupSize = GROUP_SIZE;
	float backStep  = s.collisionThreshold * s.backStepK;

	char                maxShapeNum_[6] = {0, 0, 0, 0, 0, 0};
	char                maxGroupNum_[6] = {0, 0, 0, 0, 0, 0};
	char                  shapeSize_[6] = {0, 0, 0, 0, 0, 0};
	char                  groupSize_[6] = {0, 0, 0, 0, 0, 0};
	char                    epsilon_[6] = {0, 0, 0, 0, 0, 0};
	char                   STEPSNUM_[6] = {0, 0, 0, 0, 0, 0};
	char        COLLISION_THRESHOLD_[6] = {0, 0, 0, 0, 0, 0};
	char SHADOW_COLLISION_THRESHOLD_[6] = {0, 0, 0, 0, 0, 0};
	char                  BACK_STEP_[6] = {0, 0, 0, 0, 0, 0};
	char             MAX_TRACE_DIST_[6] = {0, 0, 0, 0, 0, 0};
	char                    BOUNCES_[6] = {0, 0, 0, 0, 0, 0};
	char                   SUN_SIZE_[6] = {0, 0, 0, 0, 0, 0};

	sprintf(               maxShapeNum_, "%d"  , s.maxShapeNum             );
	sprintf(               maxGroupNum_, "%d"  , s.maxGroupNum             );
	sprintf(                 shapeSize_, "%d"  , shapeSize                 );
	sprintf(                 groupSize_, "%d"  , groupSize                 );
	sprintf(                   epsilon_, "%.2f", s.epsilon                 );
	sprintf(                  STEPSNUM_, "%d"  , s.stepsNum                );
	sprintf(       COLLISION_THRESHOLD_, "%.3f", s.collisionThreshold      );
	sprintf(SHADOW_COLLISION_THRESHOLD_, "%.3f", s.shadowCollisionThreshold);
	sprintf(                 BACK_STEP_, "%.3f", backStep                  );
	sprintf(            MAX_TRACE_DIST_, "%.3f", s.maxTraceDist            );
	sprintf(                   BOUNCES_, "%d"  , s.bounces                 );
	sprintf(                  SUN_SIZE_, "%.3f", s.sunSize                 );
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
	char* c = malloc(sizeof(char) * FRAG_HEADER_SIZE);
	memset(c, 0, FRAG_HEADER_SIZE);

	strcat(c, "// HEADER START\n");
	strcat(c, defines);
	strcat(c, uniforms);
	strcat(c, globals);
	strcat(c, consts);
	strcat(c, out);
	strcat(c, structs);
	strcat(c, "// HEADER END\n\n");

	return c;
}

char* createSDFsSource(short shapesMask) {
	char* c = malloc(sizeof(char) * FRAG_SDF_SIZE);
	memset(c, 0, FRAG_SDF_SIZE);

	strcat(c, "// SDFS START\n");
	if (shapesMask & SPHERE_MASK  ) strcat(c, sphereSDF  );
	if (shapesMask & CUBE_MASK    ) strcat(c, cubeSDF    );
	if (shapesMask & CYLINDER_MASK) strcat(c, cylinderSDF);
	if (shapesMask & TORUS_MASK   ) strcat(c, torusSDF   );
	if (shapesMask & CCONE_MASK   ) strcat(c, cconeSDF   );
	if (shapesMask & BOXFRAME_MASK) strcat(c, boxFrameSDF);
	if (shapesMask & CTORUS_MASK  ) strcat(c, ctorusSDF  );
	if (shapesMask & LINK_MASK    ) strcat(c, linkSDF    );
	if (shapesMask & PLANE_MASK   ) strcat(c, planeSDF   );
	if (shapesMask & HEXPRISM_MASK) strcat(c, hexPrismSDF);
	if (shapesMask & TRIPRISM_MASK) strcat(c, triPrismSDF);
	if (shapesMask & CAPSULE_MASK ) strcat(c, capsuleSDF );
	if (shapesMask & PYRAMID_MASK ) strcat(c, pyramidSDF );
	if (shapesMask & TRIANGLE_MASK) strcat(c, triangleDF );
	if (shapesMask & QUAD_MASK    ) strcat(c, quadDF     );
	strcat(c, "// SDFS END\n\n");

	return c;
}

char* createMapWorldSource() {
	char* c = malloc(sizeof(char) * FRAG_MAP_SIZE);
	memset(c, 0, FRAG_MAP_SIZE);

	strcat(c, "// MAP WORLD START\n");
	strcat(c, combineFuncs);

	strcat(c, mapFuncStart);
	char* fullSDF = malloc(sizeof(char) * FRAG_MAP_SIZE);
	sprintf(fullSDF, "cutM(maxM(%s, %s), minM(%s, minM(%s, %s)))",
			"map(vec4(0,1,1,0),d2Cube    (pos,vec3(0),vec3(0.9)))",
			"map(vec4(0,0,1,0),d2Sphere  (pos,vec3(0),1.2))",
			"map(vec4(0,1,0,0),d2Cylinder(pos,vec3(-1, 0, 0),vec3(1,0,0),0.5))",
			"map(vec4(0,1,0,0),d2Cylinder(pos,vec3( 0,-1, 0),vec3(0,1,0),0.5))",
			"map(vec4(0,1,0,0),d2Cylinder(pos,vec3( 0, 0,-1),vec3(0,0,1),0.5))"
			);
	strcat(c, fullSDF);
	strcat(c, mapFuncEnd);

	strcat(c, "// MAP WORLD END\n\n");

	return c;
}

char* createEndSource() {
	char* c = malloc(sizeof(char) * FRAG_END_SIZE);
	memset(c, 0, FRAG_END_SIZE);

	strcat(c, "// END START\n");
	strcat(c, normalFunc);
	strcat(c, intersectionFunc);
	strcat(c, rayMarchFunc);
	strcat(c, rayMarchShadowFunc);
	strcat(c, mainFunc);
	strcat(c, "// END END\n\n");

	return c;
}

char* createFragSource(Scene s) {
	char* c = malloc(sizeof(char) * FRAG_FULL_SIZE);
	char* vers = createVersionSource();
	char* settings = createSettingsSource(s);
	char* header = createHeaderSource();
	char* SDFs = createSDFsSource(s.shapeMask);
	char* mapWorld = createMapWorldSource();
	char* end = createEndSource();
	memset(c, 0, FRAG_FULL_SIZE);

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

	return c;
}

