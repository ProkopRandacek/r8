const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.00001;
const float MAX_TRACE_DIST = 100.0;
const int BOUNCES = 1;

const float SUN_SIZE = 0.3;

// 3D checkerboard pattern for coloring object. RN accesible only if hardcoded (like floor in mapWorld())
vec3 checkerboard(in vec3 pos) {
	pos += vec3(4.0); // there is a off by one error around y=0 || x=0, so i just move these outside the shape
	if (((int(pos.x) + int(pos.y) + int(pos.z)) % 2) == 0.0) { return vec3(0.5); }
	else { return vec3(0.7); }
}

// distance to nearest object
// returns xyz as a color of the surface and w as the distance to it
vec4 mapWorld(vec3 pos) {
	for (int i = 0; i < groupNum; i++) { d2Group(pos, i); }

	vec3 localClr = d2Groups[groupNum - 1].xyz;
	float localDist = d2Groups[groupNum - 1].w;

	// Check floor
	float dist = d2Cube(pos, vec3(0.0, -1.0, 0.0), vec3(4.0, 2.0, 4.0), 0.0);
	vec4 combined = Combine(localDist, dist, localClr, checkerboard(pos), 0, 0.0);
	localClr = combined.xyz;
	localDist = combined.w;

	return vec4(localClr, localDist);
}

// calculate normal from given point on a surface
vec3 calculateNormal(vec3 p) {
	const vec3 smol = vec3(0.00001, 0.0, 0.0);
	float x = mapWorld(p + smol.xyy).w - mapWorld(p - smol.xyy).w;
	float y = mapWorld(p + smol.yxy).w - mapWorld(p - smol.yxy).w;
	float z = mapWorld(p + smol.yyx).w - mapWorld(p - smol.yyx).w;
	return normalize(vec3(x, y, z));
}

// casts a ray
rayHit rayMarch(vec3 rayOrigin, vec3 rayDir) {
	float distTraveled = 0.0;
	float shadow = 1.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		vec4 hit = mapWorld(currentPos);

		float safeDist = hit.w;
		shadow = min(shadow, safeDist/(distTraveled * SUN_SIZE));

		if (safeDist < COLLISION_THRESHOLD) { // collision
			return rayHit(currentPos, hit.rgb, i, true, shadow);
		}
		distTraveled += safeDist;
		if (distTraveled > MAX_TRACE_DIST) { // too far
			return rayHit(vec3(0.0), vec3(0.1), i, false, shadow); // run out of trace_dist or stepsnum
		}
	}

	return rayHit(vec3(0.0), vec3(0.1), STEPSNUM, false, shadow); // run out of trace_dist or stepsnum
}

void main() {
	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 startPos, dir;
	vec3 lPoint = mix(cam[4], cam[2], uv.y);
	vec3 rPoint = mix(cam[3], cam[1], uv.y);
	vec3 point  = mix(rPoint, lPoint, uv.x);
	dir = normalize(point - cam[0]);
	startPos = cam[0];

	// cast main ray
	vec3 finalClr = vec3(0.0);

	vec3 pos = startPos;

	for (int i = 0; i < BOUNCES + 1; i++) {
		rayHit hit = rayMarch(pos, dir);

		if (i == 0) { finalClr = hit.surfaceClr; }
		else        { finalClr = finalClr * 0.5 + hit.surfaceClr * 0.5; }

		if (!hit.hit) break; // only reflect and shadown when hit a surface

		vec3 normal = calculateNormal(hit.hitPos);
		vec3 smolNormal = normal * COLLISION_THRESHOLD * 100;

		pos = hit.hitPos + smolNormal;
		dir = reflect(dir, normal);

		rayHit shd = rayMarch(pos, normalize(lightPos - pos));
		finalClr *= vec3(shd.shadow);
	}

	// output color
	outColor = vec4(finalClr, 1.0);
}
