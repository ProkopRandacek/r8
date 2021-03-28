// distance to a shape. st = shape type; i = shape index
// some shapes are commented since they are not yes used and just take up space in the binary
void d2Shape(vec3 pos, int st, int i) {
	//if (i == ignore) { return 9999.0f; }

	     if (st == 1 ) { d2Shapes[i] = d2Cube    (pos, sP(i), sS (i), sR(i)              ); } // cube
	else if (st == 2 ) { d2Shapes[i] = d2Sphere  (pos, sP(i), sR (i)                     ); } // sphere
	else if (st == 3 ) { d2Shapes[i] = d2Cylinder(pos, sP(i), sS (i), sR(i)              ); } // cylinder
	else if (st == 5 ) { d2Shapes[i] = d2Torus   (pos, sP(i), sRV(i)                     ); } // torus
	else if (st == 12) { d2Shapes[i] = d2CCone   (pos, sP(i), sS (i), sF(i, 9), sF(i, 10)); } // capped cone
}

// Distance to a group
// If A or B is a subgroup, this function needs to be called for both subgroups before calling it for this group.
void d2Group(vec3 pos, int i) {
	float aDist, bDist;
	vec4 aClr, bClr;

	if (gA(i) >= maxShapeNum) {
		aClr  = d2GroupsC[gA(i) - maxShapeNum];
		aDist = d2GroupsD[gA(i) - maxShapeNum];
	} else {
		aClr  = sC(gA(i));
		aDist = d2Shapes[gA(i)];
	}

	if (gB(i) >= maxShapeNum) {
		bClr  = d2GroupsC[gB(i) - maxShapeNum];
		bDist = d2GroupsD[gB(i) - maxShapeNum];
	} else {
		bClr  = sC(gB(i));
		bDist = d2Shapes[gB(i)];
	}

	// save the dist for later groups that contain this group.
	map c = Combine(aDist, bDist, aClr, bClr, gOp(i), gK(i));
	d2GroupsC[i] = c.clr;
	d2GroupsD[i] = c.d;
}

void d2GroupD(vec3 pos, int i) {
	float a, b;

	if (gA(i) >= maxShapeNum) { a = d2GroupsD[gA(i) - maxShapeNum]; }
	else                      { a = d2Shapes[gA(i)]; }

	if (gB(i) >= maxShapeNum) { b = d2GroupsD[gB(i) - maxShapeNum]; }
	else                      { b = d2Shapes[gB(i)]; }

	// save the dist for later groups that contain this group.
	d2GroupsD[i] = CombineD(a, b, gOp(i), gK(i));
}

