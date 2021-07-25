#version 330
// SETTINGS START
#define containerNum 16 // number of containers
#define primsNum 4 // number of primitives

#define eps 0.02

#define maxStep 256

#define collThreshold 0.001
#define shadowCollThreshold 0.001

#define backStep 0.100
#define maxTrace 30.000

#define bounces 1
#define sunSize 2.000
// SETTINGS END

// CONSTANTS START
#define shapeSize 8
// CONSTANTS end

// HEADER START
#define dot2(v) dot(v,v)
#define ndot(a, b) a.x*b.x-a.y*b.y

// Functions to find information in shapes array
#define sF(i, o) rawShapes[shapeSize * i + o]                // return float on position `o` from shape on index `i`
#define sV2(i, o) vec2(sF(i, o), sF(i, o + 1))               // vec2 from 2 floats on position from o to o + 1 from shape on index `i`
#define sV3(i, o) vec3(sF(i, o), sF(i, o + 1), sF(i, o + 2)) // vec3 from 3 floats on position from o to o + 2 from shape on index `i`

#define sP(i)  sV3(i, 0) // position
#define sP2(i) sV3(i, 7) // second position
#define sS(i)  sV3(i, 7) // scale

#define sR1(i) sF(i, 10) // first radius
#define sR2(i) sF(i, 11) // second radius

#define sC(i) vec4(sF(i, 3), sF(i, 4), sF(i, 5), sF(i, 6)) // Color

uniform float prims[shapeSize * maxShapeNum];
uniform ivec2 resolution;
uniform vec3 lightPos;
uniform vec3 viewEye;
uniform vec3 viewCenter;

const vec3 smol = vec3(epsilon, 0.0, 0.0);
const vec3 ones = vec3(1)

out vec4 outColor;

struct rayHit{vec4 hitPos,surfaceClr;}; // hitPos.w == 0 -> didnt hit else hit
struct map{vec4 clr;float d;};
// HEADER END

// SDFS START
float d2Sphere(vec3 pos,vec3 sp,float r){return length(pos-sp)-r;}

float d2Cube(vec3 pos,vec3 sp,vec3 b){vec3 p=pos-sp;vec3 q=abs(p)-b;return length(max(q,0.0))+min(max(q.x,max(q.y,q.z)),0.0);}

// SDFS END

// MAP WORLD START
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

map mapWorld(vec3 pos) {
	return cutM(map(sC(1), d2Sphere(pos, sP(1), sR(1))), map(sC(0), d2Cube(pos, sP(0), sS(0))));
}
float mapWorldD(vec3 pos) {
	return max(d2Sphere(pos, sP(1), sR(1)), -d2Cube(pos, sP(0), sS(0)));
}

// MAP WORLD END

// END START
vec3 calculateNormal(vec3 p){return normalize(vec3(mapWorldD(p+smol.xyy)-mapWorldD(p-smol.xyy),mapWorldD(p+smol.yxy)-mapWorldD(p-smol.yxy),mapWorldD(p+smol.yyx)-mapWorldD(p-smol.yyx)));}

rayHit rayMarch(vec3 ro, vec3 rd) {
	float t = 0;
	vec3 pos;

	for (int i = 0; i < STEPSNUM; i++) {
		pos = ro + t * rd;
		map hit = mapWorld(pos);
		t += hit.d;

		if (hit.d < COLLISION_THRESHOLD) { return rayHit(vec4(pos, 1), hit.clr               ); }
		if (t > MAX_TRACE_DIST)          { return rayHit(vec4(0)     , vec4(0.1, 0.1, 0.1, 0)); }
	}

	return rayHit(vec4(0), vec4(1, 1, 1, 0)); // run out of stepsnum
}

float rayMarchShadow(vec3 ro, vec3 rd, float tmax) {
	float res = 1.0;
	float t = 0.01;
	float ph = 1e10; // big, such that y = 0 on the first iteration

	for (int i=0; i<32; i++) {
		float h = mapWorld(ro + rd*t).d;

		float y = h * h / (2.0 * ph);
		float d = sqrt(h*h-y*y);
		res = min(res, SUN_SIZE * d / max(0.0, t - y));
		ph = h;

		t += h;

		if(res < 0.0001 || t > tmax) break;

	}
	res = clamp(res, 0.0, 1.0);
	return res * res * (3.0 - 2.0 * res);
}

void main() {
	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 dir = normalize(mix(mix(cam[3], cam[1], uv.y), mix(cam[4], cam[2], uv.y), uv.x) - cam[0]);
	vec3 pos = viewEye;

	vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;

	// camera from raylib magic
	vec3 pos = viewEye;
	vec3 dir = setCamera( ro, viewCenter, 0.0 ) * normalize( vec3(p.xy,2.0) );

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
