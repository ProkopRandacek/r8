// vim: filetype=c
#ifndef SHADERGEN_H
#define SHADERGEN_H
#define allMasks CUBE_MASK | SPHERE_MASK | CYLINDER_MASK | BOXFRAME_MASK | TORUS_MASK | CTORUS_MASK | LINK_MASK | PLANE_MASK | HEXPRISM_MASK | TRIPRISM_MASK | CAPSULE_MASK | CCONE_MASK | PYRAMID_MASK | TRIANGLE_MASK | QUAD_MASK

typedef enum {
	    CUBE_MASK = 1,
	  SPHERE_MASK = 2,
	CYLINDER_MASK = 4,
	BOXFRAME_MASK = 8,
	   TORUS_MASK = 16,
	  CTORUS_MASK = 32,
  	    LINK_MASK = 64,
	   PLANE_MASK = 128,
	HEXPRISM_MASK = 256,
	TRIPRISM_MASK = 512,
	 CAPSULE_MASK = 1024,
	   CCONE_MASK = 2048,
	 PYRAMID_MASK = 4096,
	TRIANGLE_MASK = 8192,
	    QUAD_MASK = 16384,
	//empty slot  = 32768
} ShapeMask;

typedef struct {
	ShapeMask shapeMask;
	int maxShapeNum;
	int maxGroupNum;
	int stepsNum;
	int bounces;
	float epsilon;
	float collisionThreshold;
	float shadowCollisionThreshold;
	float backStepK;
	float maxTraceDist;
	float sunSize;
} Scene;


char* createVertSource(void);

char* createVersionSource(void);
char* createSettingsSource(Scene s);
char* createHeaderSource(void);
char* createSDFsSource(short shapesMask);
char* createMapWorldSource(void);
char* createEndSource(void);
char* createFragSource(Scene s);

#endif
