float rayMarchShadow(vec3 ro, vec3 rd, float tmax) {
	float res = 1.0;
	float t = 0.01;
	float ph = 1e10; // big, such that y = 0 on the first iteration

	for (int i=0; i<32; i++) {
		float h = mapWorld(ro + rd*t).d;

		float y = h * h / (2.0 * ph);
		float d = sqrt(h*h-y*y);
		res = min(res, SUN_SIZE * d / max(0.0, t - y));
		ph = h;

		t += h;

		if(res < 0.0001 || t > tmax) break;

	}
	res = clamp(res, 0.0, 1.0);
	return res * res * (3.0 - 2.0 * res);
}

