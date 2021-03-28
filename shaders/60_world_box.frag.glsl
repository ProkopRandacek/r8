bool intersection(vec3 ro, vec3 rd) {
	vec3 tMin = (vec3(-4, -3, -4) - ro) / rd;
	vec3 tMax = (vec3( 4,  3,  4) - ro) / rd;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	return max(max(t1.x, t1.y), t1.z) <= min(min(t2.x, t2.y), t2.z);
}
