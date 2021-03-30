#version 330

uniform ivec2 resolution;

out vec4 outColor;

vec3 cam[5] = vec3[5](vec3(0,5,-10),vec3(-5,10,0),vec3(5,10,0),vec3(-5,0,0),vec3(5,0,0));

vec3  sPos = vec3(0, 5, 0);
vec3  sClr = vec3(0, 0.5, 1);
float sRad = 1;

const int   STEPS_NUM = 256;
const float MAX_TRACE_DIST = 15.0;
const float COLL_THRESHOLD = 0.001;

float SDF(vec3 p) { return length(p-sPos) - sRad; }

vec4 rayMarch(vec3 ro, vec3 rd) {
	float dt = 0;
	vec3 pos;

	for (int i = 0; i < STEPS_NUM; i++) {
		pos = ro + dt * rd;
		float d2s = SDF(pos);
		dt += d2s;

		if (d2s<COLL_THRESHOLD) { return vec4(pos, 1); }
		if (dt>MAX_TRACE_DIST)  { return vec4(0)     ; }
	}

	return vec4(0); // run out of stepsnum
}

vec3 norm(vec3 p) {
	const vec2 smol = vec2(0.002, 0.0);
	return normalize(vec3(
				SDF(p + smol.xyy) - SDF(p - smol.xyy),
				SDF(p + smol.yxy) - SDF(p - smol.yxy),
				SDF(p + smol.yyx) - SDF(p - smol.yyx)
			     ));
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 dir = normalize(mix(mix(cam[3], cam[1], uv.y), mix(cam[4], cam[2], uv.y), uv.x) - cam[0]);
	vec3 pos = cam[0];

	vec4 hit = rayMarch(pos, dir);

	if (hit.w == 0.0) outColor = vec4(uv, 1, 1);
	else outColor = vec4(0.5+0.5*norm(hit.xyz), 1);
}
