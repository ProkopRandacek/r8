#version 330
uniform vec2 iResolution;

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

#define maxdist 100.
//iterations trough space between portals
#define iterRm 200.
//iterations trough portals (max number of portals trough portals)
#define iterTp 20

//epsilon raymarch
#define epsRm .001
//epsilon for normal, derivative
#define epsDx .001

#define fov 1.0

#define floorheight -1.5

//material g-buffer enum
#define background 0.0
#define mPortal1   1.0
#define mPortal2   2.0
#define mFrame     3.0
#define mFloor     4.0
#define mGeometry  5.0

#define pi0 6.28318530718
#define pi  acos(-1.)
#define pi2 acos(0.)

float suv(vec2 a) { return a.x + a.y; }
float suv(vec3 a) { return dot(a, vec3(1)); }
float mav(vec2 a) { return max(a.x, a.y); }
float mav(vec3 a) { return max(max(a.x, a.y), a.z); }

//dir and up should be a vec4 quaternion instead!
//not just for performance, but also to avoid gimbal locking
struct PortalFrame {
    vec3 c; // center
    vec3 dir; // forward
    vec3 up; // up
    vec2 dims; // scale
};

struct PortalGroup {
    PortalFrame a, b;
};
PortalGroup pg1 = PortalGroup(
    PortalFrame(vec3( 0, 0, 0)
               ,vec3( 0, 0, 1)
               ,vec3( 0, 1, 0)
               ,vec2( 2, 2   )),
    PortalFrame(vec3(-2, 0, 0)
               ,vec3( 1, 0, 0)
               ,vec3( 0, 1, 0)
               ,vec2( 2, 2   ))
    );

vec2 portalsdf(vec3 u, PortalFrame p, out vec3 a) {
    vec3 r2c = u - p.c;
    vec3 w = normalize(cross(p.dir, p.up)); // right vector?
    vec3 v = cross(w, p.dir); // down vector?
    a = vec3(dot(r2c, w), dot(r2c, v), dot(r2c, p.dir));
    p.dims = p.dims / 2.;
    float d = mav(vec3(abs(a.z), abs(a.xy) - p.dims.xy));
    return vec2(d, mPortal1);
}

void portalGroupSDF(inout vec2 d, vec3 u, PortalGroup pg, out vec3 p) {
    vec3 q1, q2; // potals are structs that will tell you their "exit (*q*uit) coordinates
    vec2 d1 = portalsdf(u, pg.a, q1), d2 = portalsdf(u, pg.b, q2);
    if (d1.x < d2.x) { p = q1; d = d1; }
    else             { p = q2; d = vec2(d2.x, mPortal2); }
}

//if (distance is smaller than previous) also write textureID
void bgw(inout vec2 d, inout vec3 p, vec3 q, float h, float m) {
    if (h < d.x) {
        d = vec2(h, m);
        p = q;
    }
}

//p* and q* are "portal target bijectuive-switcheroo coordinates.
//distance field gradient
vec2 sdf(vec3 u, out vec3 p) {
    vec2 d;
    portalGroupSDF(d, u, pg1, p);
    bgw(d, p, u, max(u.y - floorheight, length(u.xz) - 10.0), mFloor); // floor
    bgw(d, p, u, length(u - vec3(2, 0, 0)) - 0.8, mGeometry); // sphere
    return d; //.y is a materialID g-buffer
}

vec2 rayMarch(vec3 u, vec3 t, out vec3 o) {
    float a = 0.0; // accumulated distance traveled
    vec2 g = sdf(u, o); // gradient at u, save travel sphere.
    for (float i = 0.0; i < iterRm; i++) {
        a += g.x;
        g = sdf(u + a * t, o);
        if (abs(g.x) < epsRm) { return vec2(a, g.y); } // hit a surface
        else if (g.x > maxdist) break; // too far from camera
    }
    return vec2(maxdist, background);
}
// having o store the position along the ray is infficient.
// it accumulates more precision loss than it gains.

vec3 ground(vec3 c) {
    c = step(mod(c, 1.0), vec3(0.25));
    return mix(vec3(0.6), vec3(0.7), abs(c.x) - c.z);
}

// teleport ro and rd trough a portal
void tp(inout vec3 ro, inout vec3 rd, inout vec3 c, PortalFrame p1, PortalFrame p2) {
    vec3 u = normalize(cross(p1.dir, p1.up));
    vec3 v = cross(u, p1.dir);
    float x = dot(rd, u), y = dot(rd, v), z = dot(rd, p1.dir);
    u = normalize(cross(p2.dir, p2.up));
    v = cross(u, p2.dir);
    vec2 s = p2.dims / p1.dims; // portal scaling
    rd = normalize(x*u*s.x
               +y*v*s.y
               +z*p2.dir); // teleport ray direction
    //above and below doesnt simplify, because u and v are vec3
    ro = p2.c
       +c.x*u*s.x
       +c.y*v*s.y
       -c.z*p2.dir
       +2.*epsRm*rd; //teleport ray origin
}

void main() {
    vec4 o = vec4(1);
    vec3 ro = 5.0 * vec3(0, 0.5, 1); //camera rotation
    vec3 w = normalize(-ro);
    vec3 u = normalize(cross(w, vec3(0, 1, 0)));
    vec3 v = cross(u, w);

    vec3 rd = normalize(fov*w
                 +(gl_FragCoord.x/iResolution.x-.5)*u
                 +(gl_FragCoord.y/iResolution.x-.5*iResolution.y/iResolution.x)*v);

    for (int i = 0; i < iterTp; i++) {
        vec3 pos; // stores position along ray
        vec2 d = rayMarch(ro, rd, pos);
        if      (d.y < 0.5) { o     = vec4(0.3, 0.5, 1, 1); break; } // cBackground
        else if (d.y < 1.5) { tp(ro, rd, pos, pg1.a, pg1.b); }       // cPortal1
        else if (d.y < 2.5) { tp(ro, rd, pos, pg1.b, pg1.a); }       // cPortal2
        else if (d.y < 3.5) { o.xyz = vec3(.2); break; }             // cPortalBorder
        else if (d.y < 4.5) { o     = vec4(ground(pos), 1); break; } // floor
        else                { o.xyz = vec3(1.0, 0.2, 0.5); }         // objects
    }
    finalColor = o;
}
