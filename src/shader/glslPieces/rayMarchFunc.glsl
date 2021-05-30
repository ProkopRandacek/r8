rayHit rayMarch(vec3 ro, vec3 rd) {
	float t = 0;
	vec3 pos;

	for (int i = 0; i < STEPSNUM; i++) {
		pos = ro + t * rd;
		map hit = mapWorld(pos);
		t += hit.d;

		if (hit.d < COLLISION_THRESHOLD) { return rayHit(vec4(pos, 1), hit.clr               ); }
		if (t > MAX_TRACE_DIST)          { return rayHit(vec4(0)     , vec4(0.1, 0.1, 0.1, 0)); }
	}

	return rayHit(vec4(0), vec4(1, 1, 1, 0)); // run out of stepsnum
}

