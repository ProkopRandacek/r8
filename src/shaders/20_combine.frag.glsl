// blend between two distances and colors
map Blend(float a, float b, vec4 colA, vec4 colB, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	float blendDst = mix(b, a, h) - k * h * (1.0 - h);
	vec4 blendCol = mix(colB, colA, h);
	return map(blendCol, blendDst);
}

map Average(float a, float b, vec4 colA, vec4 colB, float k) {
	float i = 1.0 - k;
	float c = (a * i) + (b * k);
	vec4 colC = colA * i + colB * k;
	return map(colC, c);
}

map Combine(float dstA, float dstB, vec4 colorA, vec4 colorB, int operation, float k) {
	float dst = dstA;
	vec4 color = colorA;

	if (operation == 1) { // Blend
		map blend = Blend(dstA, dstB, colorA, colorB, k);
		dst = blend.d;
		color = blend.clr;
	} else if (operation == 4) { // Average
		map avg = Average(dstA, dstB, colorA, colorB, k);
		dst = avg.d;
		color = avg.clr;
	}
	else if (operation == 0) { if ( dstB < dstA) { dst =  dstB; color = colorB; } } // Normal (min(a,  b))
	else if (operation == 2) { if (-dstB > dst)  { dst = -dstB; color = colorB; } } // Cut    (max(a, -b))
	else if (operation == 3) { if ( dstB > dst)  { dst =  dstB; color = colorB; } } // Mask   (max(a,  b))
	// planed:
	// abstraction - A is a complicated subgroup and B is a simple abstraction to use when far away from the object for optimalization.

	return map(color, dst);
}
