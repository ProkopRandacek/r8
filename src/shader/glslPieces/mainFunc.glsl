void main() {
	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 dir = normalize(mix(mix(cam[3], cam[1], uv.y), mix(cam[4], cam[2], uv.y), uv.x) - cam[0]);
	vec3 pos = cam[0];

	// test if the ray is gonna intersect with the world box, if not, just render it as a sky box
	if (!intersection(pos, dir)) {
		outColor = vec4(1, 0, 0, 1);
	} else {
		vec3 finalClr = vec3(0);
		float lastR = 1;
		rayHit hit;
		// cast main ray
		for (int i = 0; i < BOUNCES + 1; i++) {
			hit = rayMarch(pos, dir);

			finalClr = mix(hit.surfaceClr.rgb, mix(finalClr, hit.surfaceClr.rgb, lastR), min(i, 1));

			if (hit.hitPos.w == 0.) break; // only reflect and shadown when hit a surface

			lastR = hit.surfaceClr.w;

			vec3 normal = calculateNormal(hit.hitPos.xyz);
			vec3 smolNormal = normal * BACK_STEP;

			pos = hit.hitPos.xyz + smolNormal;
			dir = reflect(dir, normal);

			if (hit.surfaceClr.w == 0.) { // shadow only 100% non reflective surfaces. shadows on reflective surfaces look weird.
				finalClr *= rayMarchShadow(pos, normalize(lightPos - pos), length(lightPos - pos));
				break;
			}
		}

		// output color
		outColor = vec4(finalClr, 1.);
	}
}

