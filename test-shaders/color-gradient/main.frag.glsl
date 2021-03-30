#version 330

uniform ivec2 resolution;

out vec4 outColor;

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	outColor = vec4(uv, 1, 1);
}
