// vim: filetype=c
typedef enum ShapeType {
	GROUP, CUBE, SPHERE, CYLINDER, BOXFRAME, TORUS, CTORUS, LINK, PLANE, HEXPRISM, TRIPRISM, CAPSULE, CCONE, PYRAMID, TRIANGLE, QUAD
} ShapeType;

typedef enum OperationType {
	NORMAL, BLEND, CUT, MASK, AVERAGE
} OperationType;

// ==== start of shapes ====

typedef struct Spheres {
	Vector3 pos;
	Vector3 clr;
	float radius;
	float rv;
} Sphere;

typedef struct Cube {
	Vector3 pos;
	Vector3 clr;
	Vector3 scale;
	float roundEdge;
	float rv;
} Cube;

typedef struct BoxFrame {
	Vector3 pos;
	Vector3 clr;
	Vector3 scale;
	float width;
	float rv;
} BoxFrame;

typedef struct Torus {
	Vector3 pos;
	Vector3 clr;
	float radius;
	float fatness;
	float rv;
} Torus;

typedef struct CTorus {
	Vector3 pos;
	Vector3 clr;
	float scX; // TODO investigate what the values do
	float scY;
	float ra;
	float rb;
	float rv; // rv is refletiveness
} CTorus;

typedef struct Link {
	Vector3 pos;
	Vector3 clr;
	float length;
	float innerR;
	float outerR;
	float rv;
} Link;

typedef struct Plane {
	Vector3 pos;
	Vector3 clr;
	Vector3 n;
	float h; // TODO whats h
	float rv;
} Plane;

typedef struct HexPrism {
	Vector3 pos;
	Vector3 clr;
	float height;
	float radius;
	float rv;
} HexPrism;

typedef struct TriPrism {
	Vector3 pos;
	Vector3 clr;
	float height;
	float radius;
	float rv;
} TriPrism;

typedef struct Capsule { // Like cylinder but round
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float radius;
	float rv;
} Capsule;

typedef struct Cylinder {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float radius;
	float rv;
} Cylinder;

typedef struct CCone {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float startR;
	float endR;
	float rv;
} CCone;

typedef struct Pyramid {
	Vector3 pos;
	Vector3 clr;
	float h;
	float rv;
} Pyramid;

typedef struct Triangle {
	Vector3 a, b, c;
	Vector3 clr;
	float rv;
} Triangle;

typedef struct Quad {
	Vector3 a, b, c, d;
	Vector3 clr;
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
Sphere* sph(Vector3 pos, Vector3 clr, float radius, float rv);
Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale, float roundEdge, float rv);
Torus* tor(Vector3 pos, Vector3 clr, float innerR, float outerR, float rv);
Capsule* caps(Vector3 start, Vector3 end, Vector3 clr, float r, float rv);
Cylinder* cyl(Vector3 start, Vector3 stop, Vector3 clr, float r, float rv);
CCone* ccone(Vector3 start, Vector3 end, Vector3 clr, float startR, float endR, float rv);

// not fully implemented yet:
BoxFrame* frame(Vector3 pos, Vector3 clr, Vector3 scale, float width, float rv);
CTorus* ctor(Vector3 pos, Vector3 clr, float scX, float scY, float ra, float rb, float rv);
Link* lik(Vector3 pos, Vector3 clr, float innerR, float outerR, float length, float rv);
Plane* pln(Vector3 pos, Vector3 clr, Vector3 n, float h, float rv);
HexPrism* xprism(Vector3 pos, Vector3 clr, float h, float r, float rv);
TriPrism* tprism(Vector3 pos, Vector3 clr, float h, float r, float rv);
Pyramid* pyr(Vector3 pos, Vector3 clr, float h, float rv);
Triangle* tri(Vector3 a, Vector3 b, Vector3 c, Vector3 clr, float rv);
Quad* quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 clr, float rv);
