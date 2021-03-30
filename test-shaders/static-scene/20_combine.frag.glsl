// combine clr and distance
map Combine(float dstA, float dstB, vec4 colorA, vec4 colorB, int op, float k) {
	float h = clamp(0.5 + 0.5 * (dstB - dstA) / k, 0.0, 1.0);

	if (op == 1) {
		dstA = mix(dstB, dstA, h) - k * h * (1.0 - h);
		colorA = mix(colorA, colorB, k);
	} else if (op == 4) {
		dstA = mix(dstA, dstB, k);
		colorA = mix(colorA, colorB, k);
	}
	else if (op == 0 &&  dstB < dstA) { dstA =  dstB; colorA = colorB; }
	else if (op == 2 && -dstB > dstA) { dstA = -dstB; colorA = colorB; }
	else if (op == 3 &&  dstB > dstA) { dstA =  dstB; colorA = colorB; }

	return map(colorA, dstA);
}

// combine just distances
float CombineD(float a, float b, int op, float k) {
	float h=clamp(0.5+0.5*(b-a)/k,0,1);

	return mix(mix(mix(mix(b,a,h)-k*h*(1-h),min(a,-b),step(3,op)),mix(mix(b,a,h)-k*h*(1-h),min(a,b),step(1,op)),step(3,op)),mix(a,b,k),step(3,op));
}
