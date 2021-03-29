void main() {
	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	outColor = vec4(uv, 0.5, 1);
}
/*
