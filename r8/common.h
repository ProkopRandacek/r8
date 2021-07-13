// vim: filetype=c

typedef enum GroupType {
	gtUNION,
	gtDIFF,
	gtINTERS,
	gtBLEND,
	gtAVERAGE,
	gtAPPROXIMATE
} GroupType;

typedef enum ShapeType {
	stPRIMITIVE,
	stGROUP,
	stWRAPPER
} ShapeType;

typedef enum Primitive {
	pSPHERE,
	pCUBE,
	pTORUS,
	pCTORUS,
	pCYL,
	pCCONE
} Primitive;

typedef struct Shape {
	double f[8];
	Primitive type;
} Shape;

typedef struct Wrapper {
	void* shape;
	char* glslMod; // TODO document this
	ShapeType type;
} Wrapper;

typedef struct Group {
	void* a;
	void* b;
	ShapeType aType, bType;
	GroupType type;
	double k; // group modificator (for approximations, blend and average)
} Group;

typedef struct PortalBridge {
	// TODO
} PortalBridge;

#define __commontesttmp "yes"
