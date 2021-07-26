#version 330
// SETTINGS START
#define EPS 0.02

#define MAXSTEP 256

#define COLLTHRESHOLD 0.001

#define BACKSTEP 0.100
#define MAXTRACE 30.000
// SETTINGS END

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

//uniform float prims[shapeSize * maxShapeNum];
uniform ivec2 resolution;
uniform vec3 lightPos;
uniform vec3 viewEye;
uniform vec3 viewCenter;

const vec3 smol = vec3(EPS, 0.0, 0.0);
const vec3 ones = vec3(1);

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

map sdf(vec3 pos) {
	return minM(
			map(vec4(0, 1, 0, 1), d2Sphere(pos, vec3(0, 4, 0), 2.0)),
			map(vec4(0, 0, 1, 1), d2Sphere(pos, vec3(0), 2.0))
		   );
}
// MAP WORLD END

// END START
rayHit rayMarch(vec3 ro, vec3 rd) {
	float t = 0;
	vec3 pos;

	for (int i = 0; i < MAXSTEP; i++) {
		pos = ro + t * rd;
		map hit = sdf(pos);
		t += hit.d;

		if (hit.d < COLLTHRESHOLD) { return rayHit(vec4(pos, 1), hit.clr               ); }
		if (t > MAXTRACE)          { return rayHit(vec4(0)     , vec4(0.1, 0.1, 0.1, 0)); }
	}

	return rayHit(vec4(0), vec4(1, 0, 1, 0)); // run out of stepsnum
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr) {
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void main() {
	vec2 p = (-resolution.xy + 2.0*gl_FragCoord.xy)/resolution.y;

	// camera from raylib
	vec3 ro = viewEye;
	vec3 ta = viewCenter;

	// camera-to-world transformation
	mat3 ca = setCamera(ro, ta, 0.0);
	// ray direction
	vec3 rd = ca * normalize(vec3(p.xy,2.0) );

	vec3 finalClr = vec3(0);
	float lastR = 1;
	rayHit hit;
	// cast main ray
	hit = rayMarch(ro, rd);

	finalClr = hit.surfaceClr.rgb;

	// output color
	outColor = vec4(finalClr, 1.);
}

