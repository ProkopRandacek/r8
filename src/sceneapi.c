#include "serialization.h"
#include "sceneapi.h"
#include "settings.h"
#include "debug.h"

#include "settings.h"

extern Primitive*  shapes[SHAPE_NUM];
extern ShapeGroup* groups[GROUP_NUM];

inline void setShapeClr(int i, vec3 value) {
	if      (shapes[i]->type == CUBE)     { (    (Cube*) shapes[i]->shape)->clr = value; }
	else if (shapes[i]->type == SPHERE)   { (  (Sphere*) shapes[i]->shape)->clr = value; }
	else if (shapes[i]->type == CYLINDER) { ((Cylinder*) shapes[i]->shape)->clr = value; }
	else if (shapes[i]->type == TORUS)    { (   (Torus*) shapes[i]->shape)->clr = value; }
	else if (shapes[i]->type == CCONE)    { (   (CCone*) shapes[i]->shape)->clr = value; }
	else {
		char errMsg[61];
		sprintf(errMsg, "Unknown shape %d on index %d (setShapeClr)", shapes[i]->type, i);
		eprint(errMsg);
	}
}
