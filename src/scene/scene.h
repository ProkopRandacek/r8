// vim: filetype=c
#include "common.h"
Scene* scene(int maxShapeNum, int maxGroupNum, int stepsNum, int bounces, float maxTraceDist, float sunSize);

void updateScene(void);
void sendCamera(void);
void createObjects(void);
void sendObjects(void);

