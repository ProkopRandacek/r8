// combine clr and distance
map Combine(float dstA, float dstB, vec4 colorA, vec4 colorB, int op, float k) {
	if (op == 1) { // Blend
		float h = clamp(0.5 + 0.5 * (dstB - dstA) / k, 0.0, 1.0);
		dstA = mix(dstB, dstA, h) - k * h * (1.0 - h);
		colorA = mix(colorA, colorB, k);
	} else if (op == 4) { // Average
		dstA = mix(dstA, dstB, k);
		colorA = mix(colorA, colorB, k);
	}
	else if (op == 0) { if ( dstB < dstA) { dstA =  dstB; colorA = colorB; } } // Normal (min(a,  b))
	else if (op == 2) { if (-dstB > dstA) { dstA = -dstB; colorA = colorB; } } // Cut    (max(a, -b))
	else if (op == 3) { if ( dstB > dstA) { dstA =  dstB; colorA = colorB; } } // Mask   (max(a,  b))

	return map(colorA, dstA);
}

// combine just distances
float CombineD(float a, float b, int op, float k) {
	float h=clamp(0.5+0.5*(b-a)/k,0,1);

	     if (op == 0) { a = min(a,b);   } // Normal
	else if (op == 1) { a = mix(b,a,h)-k*h*(1-h); } // Blend
	else if (op == 2) { a = max(a,-b);  } // Cut
	else if (op == 3) { a = max(a,b);   } // Mask
	else if (op == 4) { a = mix(a,b,k); } // Average

	return a;
}
