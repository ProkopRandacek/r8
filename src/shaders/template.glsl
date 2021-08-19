#version 330

#define EPS @
#define MAX_DIST @
#define RM_ITERS @
#define MAIN_ITERS @

#define PORTAL_SIZE 11
#define PORTAL_NUM 2

uniform float portals[PORTAL_SIZE * PORTAL_NUM * 2];
uniform vec2 resolution;
uniform vec3 viewEye;
uniform vec3 viewCenter;

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
	vec3 q; // portals exit thingie
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
	// This will be later taken from uniform
	const int portalNum = 2;
	Portal portals[portalNum];
	portals[0] = Portal(
			vec3( 2, 1, 3),
			vec3( 0, 0, 1),
			vec3( 0, 1, 0),
			vec2( 1, 2   )
		   );
	portals[1] = Portal(
			vec3(-2, 1, 3),
			vec3( 1, 0, 0),
			vec3( 0, 1, 0),
			vec2( 1, 2   )
		   );

	float dist = 99999.9;
	int pi;
	vec3 q;

	for (int i = 0; i < portalNum; i++) {
		vec3 nq;
		float ndist = portalSDF(pos, portals[i], nq);
		if (ndist < dist) {
			dist = ndist;
			pi = i;
			q = nq;
		}
	}
	return portd(pi, dist, q);
}
clrd sdf(vec3 pos) {
	float d = min(
			pos.y,
			min(
				d2Cube  (pos, vec3(-2, 1, 0), vec3(1.5)),
				d2Sphere(pos, vec3( 2, 1, 0), 1.0)
			   )
		     );
	// TODO colors
	return clrd(vec4(0), d);
}

rayHit rayMarch(vec3 ro, vec3 rd) {
	vec3 pos;
	float t = 0.0;
	for (int i = 0; i < RM_ITERS; i++) {
		pos = ro + t * rd;

		clrd c = sdf(pos);
		portd p = portalsSDF(pos);

		if (c.d < p.d) {
			t += c.d;
			if (abs(c.d) < EPS) return rayHit(c.clr, pos, t, -1, vec3(0)); // hit a surface
		} else {
			t += p.d;
			if (abs(p.d) < EPS) return rayHit(c.clr, pos, t, p.pi, p.q); // hit a surface
		}
		if (t > MAX_DIST) break; // too far from camera
	}
	return rayHit(vec4(0), pos, MAX_DIST, -1, vec3(0));
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr) {
	vec3 cw = normalize(ta - ro);
	vec3 cp = vec3(sin(cr), cos(cr), 0.0);
	vec3 cu = normalize(cross(cw, cp));
	vec3 cv = normalize(cross(cu, cw));
	return mat3(cu, cv, cw);
}

void main() {
	const int portalNum = 2;
	Portal portals[portalNum];
	portals[0] = Portal(
			vec3( 2, 1, 3),
			vec3( 0, 0, 1),
			vec3( 0, 1, 0),
			vec2( 1, 2   )
		   );
	portals[1] = Portal(
			vec3(-2, 1, 3),
			vec3( 1, 0, 0),
			vec3( 0, 1, 0),
			vec2( 1, 2   )
		   );

	vec2 p = (2.0 * gl_FragCoord.xy - resolution.xy) / resolution.y;

	vec3 ro = viewEye;
	vec3 ta = viewCenter;

	vec3 clr = vec3(0);

	mat3 ca = setCamera(ro, ta, 0.0);
	const float fl = 2.0; // focal length
	vec3 rd = ca * normalize(vec3(p, fl));

	for (int i = 0; i < MAIN_ITERS; i++) {
		rayHit hit = rayMarch(ro, rd);
		if (hit.pi < 0) { // didn't hit a portal
			clr = vec3(hit.dist / MAX_DIST); // render depth texture
			break;
		} else { // did hit a portal
			int b;

			if (hit.pi % 2 == 1) {
				b = hit.pi - 1;
			} else {
				b = hit.pi + 1;
			}

			tp(ro, rd, hit.q, portals[hit.pi], portals[b]);
			continue;
		}
	}

	finalColor = vec4(clr, 1.0);
}
