#define STEPSNUM 512
#define COLLISION_THRESHOLD 0.001 // COLLISION_THRESHOLD has HUGE impact on FPS
#define BACK_STEP COLLISION_THRESHOLD * 20
#define MAX_TRACE_DIST 15.0
#define BOUNCES 1
#define SUN_SIZE 1.0

// 3D checkerboard pattern for coloring object. RN accesible only if hardcoded (like floor in mapWorld())
#define checkerboard(p) mix(vec4(0.5, 0.5, 0.5, 1), vec4(0.7, 0.7, 0.7, 0), max(sign(mod(dot(floor(p), ones), 2.0)), 0.0))

// distance to nearest object
map mapWorld(vec3 pos) {
	for (int i = 0; i < shapeNum; i++) { d2Shape(pos, sT(i), i); }
	for (int i = 0; i < groupNum; i++) { d2Group(pos,        i); }

	vec4  localClr  = d2GroupsC[groupNum - 1];
	float localDist = d2GroupsD[groupNum - 1];

	// combine with floor and return
	return Combine(localDist, d2Cube(pos, vec3(0.0, -1.0, 0.0), vec3(4.0, 2.0, 4.0), 0.0), localClr, checkerboard(pos), 0, 0.0);
}

// JUST distance to nearest object. no color. used for calculateNormal who calls mapWorld 6 times and doesnt need the color
float mapWorldD(vec3 pos) {
	for (int i = 0; i < shapeNum; i++) { d2Shape (pos, sT(i), i); }
	for (int i = 0; i < groupNum; i++) { d2GroupD(pos,        i); }

	float localDist = d2GroupsD[groupNum - 1];

	// Combine with floor and return
	return CombineD(localDist, d2Cube(pos, vec3(0.0, -1.0, 0.0), vec3(4.0, 2.0, 4.0), 0.0), 0, 0.0);
}

// calculate normal from given point on a surface
vec3 calculateNormal(vec3 p) {
	return normalize(vec3(
				mapWorldD(p + smol.xyy) - mapWorldD(p - smol.xyy),
				mapWorldD(p + smol.yxy) - mapWorldD(p - smol.yxy),
				mapWorldD(p + smol.yyx) - mapWorldD(p - smol.yyx)
			     ));
}

// casts a ray
rayHit rayMarch(vec3 rayOrigin, vec3 rayDir) {
	float distTraveled = 0;
	vec3 currentPos;
	map hit;

	for (int i = 0; i < STEPSNUM; i++) {
		currentPos = rayOrigin + distTraveled * rayDir;
		hit = mapWorld(currentPos);
		distTraveled += hit.d;

		if (hit.d < COLLISION_THRESHOLD)   { return rayHit(currentPos, hit.clr               , true ); }
		if (distTraveled > MAX_TRACE_DIST) { return rayHit(vec3(0)   , vec4(0.1, 0.1, 0.1, 0), false); }
	}

	return rayHit(vec3(0), vec4(1, 1, 1, 0), false); // run out of stepsnum
}

// light version of rayMarch that cares ony about the shadow info
float rayMarchShadow(vec3 ro, vec3 rd) {
	float t = 0;
	float shadow = 1;
	for (int i = 0; i < STEPSNUM; i++) {
		vec3 pos = ro + t * rd;
		float h = mapWorldD(pos);
		shadow = min(shadow, h / (t * SUN_SIZE));
		t += h;
		if (h < COLLISION_THRESHOLD || t > MAX_TRACE_DIST) break;
	}
	return shadow;
}

void main() {
	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 pos, dir;
	dir = normalize(mix(mix(cam[3], cam[1], uv.y), mix(cam[4], cam[2], uv.y), uv.x) - cam[0]);
	pos = cam[0];

	// test if the ray is gonna intersect with the world box, if not, just render it as a sky box
	if (!intersection(pos, dir)) {
		outColor = vec4(0.12, 0.12, 0.12, 1);
	} else {
		vec3 finalClr, normal, smolNormal;
		float lastR;
		rayHit hit;
		// cast main ray
		for (int i = 0; i < BOUNCES + 1; i++) {
			hit = rayMarch(pos, dir);

			finalClr = mix(hit.surfaceClr.rgb, mix(finalClr, hit.surfaceClr.rgb, lastR), min(i, 1));

			if (!hit.hit) break; // only reflect and shadown when hit a surface

			lastR = hit.surfaceClr.w;

			normal = calculateNormal(hit.hitPos);
			smolNormal = normal * BACK_STEP;

			pos = hit.hitPos + smolNormal;
			dir = reflect(dir, normal);

			if (hit.surfaceClr.w == 0.0) { // shadow only 100% non reflective surfaces. shadows on reflective surfaces look weird.
				finalClr *= vec3(rayMarchShadow(pos, normalize(lightPos - pos)));
				break;
			}
		}

		// output color
		outColor = vec4(finalClr, 1.0);
	}
}
// */

