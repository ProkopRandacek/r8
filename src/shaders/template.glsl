#version 330

#define EPS @
#define MAX_DIST @
#define RM_ITERS @
#define MAIN_ITERS @

#define PORTAL_SIZE 4
#define PORTAL_NUM 2

#define PRIM_SIZE 12
#define PRIM_NUM @

#define GROUP_NUM @

// Functions to find information in the float arrays
#define sF(i, o) prims[PRIM_SIZE * i + o]                    // return float on position `o` from shape on index `i`
#define sV2(i, o) vec2(sF(i, o), sF(i, o + 1))               // vec2 from 2 floats on positions from o to o + 1 from shape on index `i`
#define sV3(i, o) vec3(sF(i, o), sF(i, o + 1), sF(i, o + 2)) // vec3 from 3 floats on positions from o to o + 2 from shape on index `i`

#define sP(i)  sV3(i, 0) // position
#define sP2(i) sV3(i, 7) // second position
#define sS(i)  sV3(i, 7) // scale

#define sR1(i) sF(i, 10) // first radius
#define sR2(i) sF(i, 11) // second radius

#define sC(i) vec4(sV3(i, 3), sF(i, 6)) // Color

#define gK(i) groups[i]

uniform vec3 portals[PORTAL_SIZE * PORTAL_NUM];
uniform float prims[PRIM_SIZE * PRIM_NUM];
uniform float groups[GROUP_NUM];
uniform vec2 resolution;
uniform vec3 viewEye;
uniform vec3 viewCenter;

//uniform float time;

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

float d2Sphere(vec3 pos,vec3 sp,float r){return length(pos-sp)-r;}
float d2Cube(vec3 pos,vec3 sp,vec3 b){vec3 p=pos-sp;vec3 q=abs(p)-b;return length(max(q,0.0))+min(max(q.x,max(q.y,q.z)),0.0);}

float mav(vec3 a) { return max(max(a.x, a.y), a.z); }

struct Portal {
	vec3 c;    // center
	vec3 dir;  // forward unit vector
	vec3 up;   // up unit vector
	vec2 dims; // scale
};

struct rayHit {
	vec4 clr;
	vec3 pos;
	float dist;
	int pi; // portal index, -1 if didn't hit portal
	vec3 q; // portal's exit thingie
};

struct clrd {
	vec4 clr; // color of an object
	float d; // distance to that object
};

struct portd {
	int pi; // portal's index
	float d; // distance to the portal
	vec3 q; // portal's exit thingie
};

clrd u(clrd a, clrd b) { // min(a, b)
	if (a.d < b.d) return a;
	return b;
}

clrd d(clrd a, clrd b) { // min(a, -b)
	if (a.d < -b.d) return a;
	return b;
}

clrd i(clrd a, clrd b) { // max(a, b)
	if (a.d > b.d) return a;
	return b;
}

clrd a(float k, clrd a, clrd b) { // mix(a, b, k)
	return clrd(
			mix(a.clr, b.clr, k),
			mix(a.d  , b.d  , k)
		   );
}

clrd b(float k, clrd a, clrd b) { // smin(a, b, k)
	float h = max(k - abs(a.d - b.d), 0.0) / k;
	float blendDist = min(a.d, b.d) - h * h * h * k * (1.0 / 6.0);
	vec4 blendClr = vec4(mix(a.clr.rgb, b.clr.rgb, k), 1.0);

	return clrd(blendClr, blendDist);
}

clrd x(float k, clrd a, clrd b) {
	// TODO
	return a;
}

Portal getPortal(int i) {
	return Portal(
			portals[i * PORTAL_SIZE + 0],
			portals[i * PORTAL_SIZE + 1],
			portals[i * PORTAL_SIZE + 2],
			portals[i * PORTAL_SIZE + 3].xy
		     );
}

float portalSDF(vec3 u, Portal p, out vec3 a) {
	vec3 r2c = u - p.c;
	vec3 w = normalize(cross(p.dir, p.up)); // right vector?
	vec3 v = cross(w, p.dir); // down vector?
	a = vec3(dot(r2c, w), dot(r2c, v), dot(r2c, p.dir));
	//p.dims = p.dims / 2.0;
	float d = mav(vec3(abs(a.z), abs(a.xy) - p.dims.xy));
	return d;
}

// teleport ro and rd trough a portal
void tp(inout vec3 ro, inout vec3 rd, inout vec3 pos, Portal p1, Portal p2) {
	vec3 u = normalize(cross(p1.dir, p1.up));
	vec3 v = cross(u, p1.dir);
	float x = dot(rd, u), y = dot(rd, v), z = dot(rd, p1.dir);
	u = normalize(cross(p2.dir, p2.up));
	v = cross(u, p2.dir);
	vec2 s = p2.dims / p1.dims; // portal scaling
	rd = normalize(x*u*s.x
			+y*v*s.y
			+z*p2.dir); // teleport ray direction
	// above and below doesnt simplify, because u and v are vec3
	ro = p2.c
		+pos.x*u*s.x
		+pos.y*v*s.y
		-pos.z*p2.dir
		+2.*EPS*rd; //teleport ray origin
}

portd portalsSDF(inout vec3 pos) {
	float dist = 99999.9;
	int pi;
	vec3 q;
	for (int i = 0; i < PORTAL_NUM; i++) {
		vec3 nq;
		float ndist = portalSDF(pos, getPortal(i), nq);
		if (ndist < dist) {
			dist = ndist;
			pi = i;
			q = nq;
		}
	}
	return portd(pi, dist, q);
}

clrd sdf(vec3 pos) {
	return @;
}

rayHit rayMarch(vec3 ro, vec3 rd) {
	vec3 pos;
	float t = 0.0;
	for (int i = 0; i < RM_ITERS; i++) {
		pos = ro + t * rd;

		clrd c = sdf(pos);
		//portd p = portd(1, 9999999.9, vec3(0));
		portd p = portalsSDF(pos);

		if (c.d < p.d) {
			t += c.d;
			if (abs(c.d) < EPS) return rayHit(c.clr, pos, t, -1, vec3(0)); // hit a surface
		} else {
			t += p.d;
			if (abs(p.d) < EPS) return rayHit(c.clr, pos, t, p.pi, p.q); // hit a portal
		}
		if (t > MAX_DIST) break; // too far from camera
	}
	return rayHit(vec4(0, 0.2, 0.4, 0.0), pos, MAX_DIST, -2, vec3(0));
}

vec3 calcNormal(vec3 pos) {
	float c = sdf(pos).d;
	vec2 eps_zero = vec2(EPS, 0.0);
	return normalize(vec3(
				sdf(pos + eps_zero.xyy).d,
				sdf(pos + eps_zero.yxy).d,
				sdf(pos + eps_zero.yyx).d
			     ) - c);
}

vec3 debugTexture(vec3 p) {
	return mix(
			vec3(0.1328, 0.1562, 0.1914),
			vec3(1.0000, 0.4492, 0.0820),
			abs(abs(step(mod(p.x, 0.5), 0.25)-step(mod(p.z, 0.5), 0.25))-step(mod(p.y, 0.5), 0.25))
		  );
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr) {
	vec3 cw = normalize(ta - ro);
	vec3 cp = vec3(sin(cr), cos(cr), 0.0);
	vec3 cu = normalize(cross(cw, cp));
	vec3 cv = normalize(cross(cu, cw));
	return mat3(cu, cv, cw);
}

void main() {
	vec2 p = (2.0 * gl_FragCoord.xy - resolution.xy) / resolution.y;

	vec3 ro = viewEye;
	vec3 ta = viewCenter;

	vec3 clr = vec3(0);

	mat3 ca = setCamera(ro, ta, 0.0);
	const float fl = 2.0; // focal length
	vec3 rd = ca * normalize(vec3(p, fl));

	for (int i = 0; i < MAIN_ITERS; i++) {
		rayHit hit = rayMarch(ro, rd);
		if (hit.pi == -1) { // didn't hit a portal
			vec3 n = calcNormal(hit.pos);
			vec3 l = normalize(vec3(1, 0.5, 1));

			float NoL = max(dot(n, l), 0.0);
			vec3 LDirectional = vec3(1.80, 1.27, 0.99) * NoL;
			vec3 LAmbient = vec3(0.03, 0.04, 0.1);
			vec3 diffuse = hit.clr.rgb * (LDirectional + LAmbient);

			clr = diffuse;

			break;
		} else if (hit.pi == -2) { // hit a sky
			clr = hit.clr.rgb;
			break;
		} else { // did hit a portal
#if 0
			int b;

			if (hit.pi % 2 == 1) { b = hit.pi - 1; }
			else { b = hit.pi + 1; }

			tp(ro, rd, hit.q, getPortal(hit.pi), getPortal(b));
			continue;
#endif
			clr = debugTexture(hit.pos);
			break;
		}
	}

	finalColor = vec4(clr, 1.0);
}

