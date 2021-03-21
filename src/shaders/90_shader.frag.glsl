const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.00001;
const float MAX_TRACE_DIST = 100.0;
const int BOUNCES = 1;

const float SUN_SIZE =1.0;

// 3D checkerboard pattern for coloring object. RN accesible only if hardcoded (like floor in mapWorld())
vec4 checkerboard(in vec3 pos) {
	pos += vec3(4.0); // there is a off by one error around y=0 || x=0, so i just move these outside the shape
	if (((int(pos.x) + int(pos.y) + int(pos.z)) % 2) == 0.0) { return vec4(vec3(0.5), 1); }
	else { return vec4(vec3(0.7), 0); }
}
// distance to nearest object
// returns xyz as a color of the surface and w as the distance to it
map mapWorld(vec3 pos) {
	for (int i = 0; i < groupNum; i++) { d2Group(pos, i); }

	vec4 localClr = d2Groups[groupNum - 1].clr;
	float localDist = d2Groups[groupNum - 1].d;

	// Check floor
	float dist = d2Cube(pos, vec3(0.0, -1.0, 0.0), vec3(4.0, 2.0, 4.0), 0.0);
	map combined = Combine(localDist, dist, localClr, checkerboard(pos), 0, 0.0);
	localClr = combined.clr;
	localDist = combined.d;

	return map(localClr, localDist);
}

// calculate normal from given point on a surface
vec3 calculateNormal(vec3 p) {
	const vec3 smol = vec3(0.00001, 0.0, 0.0);
	float x = mapWorld(p + smol.xyy).d - mapWorld(p - smol.xyy).d;
	float y = mapWorld(p + smol.yxy).d - mapWorld(p - smol.yxy).d;
	float z = mapWorld(p + smol.yyx).d - mapWorld(p - smol.yyx).d;
	return normalize(vec3(x, y, z));
}

// casts a ray
rayHit rayMarch(vec3 rayOrigin, vec3 rayDir) {
	float distTraveled = 0.0;
	float shadow = 1.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		map hit = mapWorld(currentPos);

		float safeDist = hit.d;
		shadow = min(shadow, safeDist/(distTraveled * SUN_SIZE));

		if (safeDist < COLLISION_THRESHOLD) { // collision
			return rayHit(currentPos, hit.clr, i, true, shadow);
		}
		distTraveled += safeDist;
		if (distTraveled > MAX_TRACE_DIST) { // too far
			return rayHit(vec3(0.0), vec4(vec3(0.1), 0), i, false, shadow); // run out of trace_dist or stepsnum
		}
	}

	return rayHit(vec3(0.0), vec4(vec3(0.1), 0), STEPSNUM, false, shadow); // run out of trace_dist or stepsnum
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
	float lastR = 0.0;

	for (int i = 0; i < BOUNCES + 1; i++) {
		rayHit hit = rayMarch(pos, dir);
		float r = hit.surfaceClr.w;

		if (i == 0) {
			finalClr = hit.surfaceClr.rgb;
		} else {
			finalClr = finalClr * (1-lastR) + hit.surfaceClr.rgb * lastR;
		}
		lastR = r;

		if (!hit.hit) break; // only reflect and shadown when hit a surface

		vec3 normal = calculateNormal(hit.hitPos);
		vec3 smolNormal = normal * COLLISION_THRESHOLD * 100;

		pos = hit.hitPos + smolNormal;
		dir = reflect(dir, normal);

		rayHit shd = rayMarch(pos, normalize(lightPos - pos));
		finalClr *= vec3(shd.shadow);

		if (r == 0.0) break;
	}

	// output color
	outColor = vec4(finalClr, 1.0);
}
