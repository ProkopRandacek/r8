// vim: filetype=c

typedef enum ShapeType {
	GROUP, CUBE, SPHERE, CYLINDER, BOXFRAME, TORUS, CTORUS, LINK, PLANE, HEXPRISM, TRIPRISM, CAPSULE, CCONE, PYRAMID, TRIANGLE, QUAD
} ShapeType;

typedef enum OperationType {
	NORMAL, BLEND, CUT, MASK, AVERAGE
} OperationType;

// ==== start of shapes ====

typedef struct Spheres {
	vec3 pos;
	vec3 clr;
	float radius;
	float rv;
} Sphere;

typedef struct Cube {
	vec3 pos;
	vec3 clr;
	vec3 scale;
	float roundEdge;
	float rv;
} Cube;

typedef struct BoxFrame {
	vec3 pos;
	vec3 clr;
	vec3 scale;
	float width;
	float rv;
} BoxFrame;

typedef struct Torus {
	vec3 pos;
	vec3 clr;
	float radius;
	float fatness;
	float rv;
} Torus;

typedef struct CTorus {
	vec3 pos;
	vec3 clr;
	float scX; // TODO investigate what the values do
	float scY;
	float ra;
	float rb;
	float rv; // rv is refletiveness
} CTorus;

typedef struct Link {
	vec3 pos;
	vec3 clr;
	float length;
	float innerR;
	float outerR;
	float rv;
} Link;

typedef struct Plane {
	vec3 pos;
	vec3 clr;
	vec3 n;
	float h; // TODO whats h
	float rv;
} Plane;

typedef struct HexPrism {
	vec3 pos;
	vec3 clr;
	float height;
	float radius;
	float rv;
} HexPrism;

typedef struct TriPrism {
	vec3 pos;
	vec3 clr;
	float height;
	float radius;
	float rv;
} TriPrism;

typedef struct Capsule { // Like cylinder but round
	vec3 start;
	vec3 end;
	vec3 clr;
	float radius;
	float rv;
} Capsule;

typedef struct Cylinder {
	vec3 start;
	vec3 end;
	vec3 clr;
	float radius;
	float rv;
} Cylinder;

typedef struct CCone {
	vec3 start;
	vec3 end;
	vec3 clr;
	float startR;
	float endR;
	float rv;
} CCone;

typedef struct Pyramid {
	vec3 pos;
	vec3 clr;
	float h;
	float rv;
} Pyramid;

typedef struct Triangle {
	vec3 a, b, c;
	vec3 clr;
	float rv;
} Triangle;

typedef struct Quad {
	vec3 a, b, c, d;
	vec3 clr;
	float rv;
} Quad;

// ==== end of shapes ====

typedef struct Primitive {
	ShapeType type;
	void* shape;
} Primitive;

typedef struct ShapeGroup {
	ShapeType ta;
	ShapeType tb;
	OperationType op;
	float k; // modifier for operation.
	int a;
	int b;
} ShapeGroup;

Primitive* prmv(ShapeType type, void* shape);
ShapeGroup* group(ShapeType ta, int a, ShapeType tb, int b, OperationType op, float k);

// used:
Sphere* sph(vec3 pos, vec3 clr, float radius, float rv);
Cube* cube(vec3 pos, vec3 clr, vec3 scale, float roundEdge, float rv);
Torus* tor(vec3 pos, vec3 clr, float innerR, float outerR, float rv);
Capsule* caps(vec3 start, vec3 end, vec3 clr, float r, float rv);
Cylinder* cyl(vec3 start, vec3 stop, vec3 clr, float r, float rv);
CCone* ccone(vec3 start, vec3 end, vec3 clr, float startR, float endR, float rv);

// not fully implemented yet:
BoxFrame* frame(vec3 pos, vec3 clr, vec3 scale, float width, float rv);
CTorus* ctor(vec3 pos, vec3 clr, float scX, float scY, float ra, float rb, float rv);
Link* lik(vec3 pos, vec3 clr, float innerR, float outerR, float length, float rv);
Plane* pln(vec3 pos, vec3 clr, vec3 n, float h, float rv);
HexPrism* xprism(vec3 pos, vec3 clr, float h, float r, float rv);
TriPrism* tprism(vec3 pos, vec3 clr, float h, float r, float rv);
Pyramid* pyr(vec3 pos, vec3 clr, float h, float rv);
Triangle* tri(vec3 a, vec3 b, vec3 c, vec3 clr, float rv);
Quad* quad(vec3 a, vec3 b, vec3 c, vec3 d, vec3 clr, float rv);
