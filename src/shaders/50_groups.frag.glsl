// Functions to find information in groups array
float gF(int i, int o) { return rawGroups[groupSize * i + o]; } // returns float on position `o` from group on index `i`

int   gAT(int i) { return int(gF(i, 0)); } // A shape type
int   gA (int i) { return int(gF(i, 1)); } // A shape/group index
int   gBT(int i) { return int(gF(i, 2)); } // B shape type
int   gB (int i) { return int(gF(i, 3)); } // B shape/group index
int   gOp(int i) { return int(gF(i, 4)); } // group operation
float gK (int i) { return     gF(i, 5) ; } // group operation modifier

// Functions to find information in shapes array
float sF(int i, int o) { return rawShapes[shapeSize * i + o]; } // return float on position `o` from shape on index `i`
vec3  sV(int i, int o) { return vec3(sF(i, o + 0), sF(i, o + 1), sF(i, o + 2)); } //vector from 3 floats on position from o to o + 2 from shape on index `i`

float sR (int i) { return sF(i, 9); } // radius
vec3  sP (int i) { return sV(i, 0); } // position
vec3  sC (int i) { return sV(i, 3); } // color
vec3  sS (int i) { return sV(i, 6); } // scale
vec2  sRV(int i) { return vec2(sF(i, 9), sF(i, 10)); } // radius vector

// distance to a shape. st = shape type; i = shape index
// some shapes are commented since they are not yes used and just take up space in the binary
float d2Shape(vec3 pos, int st, int i) {
	float dist;
	if      (st == 1 ) { dist = d2Cube    (pos, sP(i), sS(i)    , sR(i)               ); } // cube
	else if (st == 2 ) { dist = d2Sphere  (pos, sP(i), sR(i)                          ); } // sphere
	else if (st == 3 ) { dist = d2Cylinder(pos, sP(i), sS(i)    , sR(i)               ); } // cylinder
//	else if (st == 4 ) { dist = d2BoxFrame(pos, sP(i), sS(i)    , sR(i)               ); } // box frame
	else if (st == 5 ) { dist = d2Torus   (pos, sP(i), sRV(i)                         ); } // torus
//	else if (st == 6 ) { dist = d2CTorus  (pos, sP(i), sRV(i)   , sF(i, 11), sF(i, 12)); } // ctorus
//	else if (st == 7 ) { dist = d2Link    (pos, sP(i), sF(i, 11), sF(i, 9 ), sF(i, 10)); } // link
//	else if (st == 8 ) { dist = d2Plane   (pos, sP(i), sS(i)    , sR(i)               ); } // plane
//	else if (st == 9 ) { dist = d2HexPrism(pos, sP(i), sRV(i)                         ); } // hexprism
//	else if (st == 10) { dist = d2TriPrism(pos, sP(i), sRV(i)                         ); } // triprism
//	else if (st == 11) { dist = d2Capsule (pos, sP(i), sS(i)    , sR(i)               ); } // capsule
	else if (st == 12) { dist = d2CCone   (pos, sP(i), sS(i)    , sF(i, 9 ), sF(i, 10)); } // capped cone
//	else if (st == 13) { dist = d2Pyramid (pos, sP(i), sR(i)                          ); } // pyramid
	return dist;
}

// Distance to a group
// If A or B is a subgroup, this function needs to be called for both subgroups before calling it for this group.
void d2Group(in vec3 pos, in int i) {
	float aDist, bDist;
	vec3 aClr, bClr;

	int aType = gAT(i);
	int bType = gBT(i);

	if (aType == 0) {
		vec4 s = d2Groups[gA(i)];
		aDist = s.w; aClr = s.rgb;
	} else {
		aClr = sC(gA(i));
		aDist = d2Shape(pos, aType, gA(i));
	}

	if (bType == 0) {
		vec4 s = d2Groups[gB(i)];
		bDist = s.w; bClr = s.rgb;
	} else {
		bClr = sC(gB(i));
		bDist = d2Shape(pos, bType, gB(i));
	}

	// save the dist for later groups that contain this group.
	d2Groups[i] = Combine(aDist, bDist, aClr, bClr, gOp(i), gK(i));
}
