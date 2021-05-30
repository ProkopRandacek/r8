map maxM(map a, map b) {
	if (a.d < b.d) return b;
	return a;
}

map minM(map a, map b) {
	if (a.d > b.d) return b;
	return a;
}
map cutM(map a, map b) {
	b.d = -b.d;
	if (a.d < b.d) return b;
	return a;
}
map avgM(float k, map a, map b) {
	return map(mix(a.clr, b.clr, k), mix(a.d, b.d, k));
}

