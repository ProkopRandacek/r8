// vim: filetype=c

const char* rayMarchFunc =
"rayHit rayMarch(vec3 ro, vec3 rd) {\n"
"        float t = 0;\n"
"        vec3 pos;\n"

"        for (int i = 0; i < STEPSNUM; i++) {\n"
"                pos = ro + t * rd;\n"
"                map hit = mapWorld(pos);\n"
"                t += hit.d;\n"

"                if (hit.d < COLLISION_THRESHOLD) { return rayHit(vec4(pos, 1), hit.clr               ); }\n"
"                if (t > MAX_TRACE_DIST)          { return rayHit(vec4(0)     , vec4(0.1, 0.1, 0.1, 0)); }\n"
"        }\n"

"        return rayHit(vec4(0), vec4(1, 1, 1, 0)); // run out of stepsnum\n"
"}\n";

const char* rayMarchShadowFunc =
"float rayMarchShadow(vec3 ro, vec3 rd) {\n"
"        float res = 1;\n"
"        float ph = 1e20;\n"
"        float t = 0;\n"
"        for (int i = 0; i < STEPSNUM; i++) {\n"
"                float h = mapWorldD(ro + rd * t);\n"
"                if (h < SHADOW_COLLISION_THRESHOLD) return 0.0;\n"
"                float y = h*h/(2.0*ph);\n"
"                float d = sqrt(h*h-y*y);\n"
"                res = min(res, d / max(0, t-y) * SUN_SIZE);\n"
"                ph = h;\n"
"                t += h;\n"
"                if (t > MAX_TRACE_DIST) return res;\n"
"        }\n"
"        return res;\n"
"}\n";
