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

"        return rayHit(vec4(0), vec4(1, 1, 1, 0));\n" // run out of stepsnum
"}\n";

const char* rayMarchShadowFunc =
"float rayMarchShadow(vec3 ro, vec3 rd, float tmax) {\n"
"	float res = 1.0;\n"
"	float t = 0.01;\n"
"	float ph = 1e10;\n" // big, such that y = 0 on the first iteration

"	for( int i=0; i<32; i++ )\n"
"	{\n"
"		float h = mapWorld( ro + rd*t ).d;\n"

"		float y = h*h/(2.0*ph);\n"
"		float d = sqrt(h*h-y*y);\n"
"		res = min(res,SUN_SIZE*d/max(0.0,t-y));\n"
"		ph = h;\n"

"		t += h;\n"

"		if( res<0.0001 || t>tmax ) break;\n"

"	}\n"
"	res = clamp( res, 0.0, 1.0 );\n"
"	return res*res*(3.0-2.0*res);\n"
"}\n";

