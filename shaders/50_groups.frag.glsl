// distance to a shape. st = shape type; i = shape index
// some shapes are commented since they are not yes used and just take up space in the binary
void d2Shape(vec3 pos, int st, int i) {
	     if (st == 1 ) { d2Shapes[i] = d2Cube    (pos, sP(i), sS (i), sR(i)              ); } // cube
	else if (st == 2 ) { d2Shapes[i] = d2Sphere  (pos, sP(i), sR (i)                     ); } // sphere
	else if (st == 3 ) { d2Shapes[i] = d2Cylinder(pos, sP(i), sS (i), sR(i)              ); } // cylinder
	else if (st == 5 ) { d2Shapes[i] = d2Torus   (pos, sP(i), sRV(i)                     ); } // torus
	else if (st == 12) { d2Shapes[i] = d2CCone   (pos, sP(i), sS (i), sF(i, 9), sF(i, 10)); } // capped cone
}

// Distance to a group
// If A or B is a subgroup, this function needs to be called for both subgroups before calling it for this group.
void d2Group(vec3 pos, int i) {
	vec4  aC = mix(sC(gA(i))      , d2GroupsC[gA(i) - maxShapeNum], step(maxShapeNum, gA(i)));
	float aD = mix(d2Shapes[gA(i)], d2GroupsD[gA(i) - maxShapeNum], step(maxShapeNum, gA(i)));
	vec4  bC = mix(sC(gB(i))      , d2GroupsC[gB(i) - maxShapeNum], step(maxShapeNum, gB(i)));
	float bD = mix(d2Shapes[gB(i)], d2GroupsD[gB(i) - maxShapeNum], step(maxShapeNum, gB(i)));

	map c = Combine(aD, bD, aC, bC, gOp(i), gK(i));
	d2GroupsC[i] = c.clr;
	d2GroupsD[i] = c.d;
}

void d2GroupD(vec3 pos, int i) {
	float a = mix(d2Shapes[gA(i)], d2GroupsD[gA(i) - maxShapeNum], step(maxShapeNum, gA(i)));
	float b = mix(d2Shapes[gB(i)], d2GroupsD[gB(i) - maxShapeNum], step(maxShapeNum, gB(i)));

	d2GroupsD[i] = CombineD(a, b, gOp(i), gK(i));
}

