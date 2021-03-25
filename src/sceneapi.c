#include "serialization.h"
#include "sceneapi.h"
#include "settings.h"
#include "debug.h"

#include "settings.h"

extern Shape*      shapes[SHAPE_NUM];
extern ShapeGroup* groups[GROUP_NUM];

inline void setShapeClr(int i, vec3 value) {
	shapes[i]->clr = value;
}
