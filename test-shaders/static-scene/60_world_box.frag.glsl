bool intersection(vec3 ro, vec3 rd) {
	const vec3 box = vec3(4, 3, 4);
	vec3 tMin = (-box - ro) / rd;
	vec3 tMax = (box - ro) / rd;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	return max(max(t1.x, t1.y), t1.z) <= min(min(t2.x, t2.y), t2.z);
}

