// Signed Distance Functions
// =========================

// Source of the following functions: http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
// =====================================================================================================

#define dot2(v) dot(v,v)
#define ndot(a, b) a.x*b.x-a.y*b.y

// Distance Functions
// ==================

float d2Sphere     (vec3 pos, vec3 sp, float r)                   { return length(pos-sp)-r; }
float d2Cube       (vec3 pos, vec3 sp, vec3 b, float r)           { vec3 p=pos-sp;vec3 q=abs(p)-b;return length(max(q,0.0))+min(max(q.x,max(q.y,q.z)),0.0)-r; }
float d2Torus      (vec3 pos, vec3 sp, vec2 t)                    { vec3 p=pos-sp;vec2 q=vec2(length(p.xz)-t.x,p.y);return length(q)-t.y; }
float d2CCone      (vec3 p  , vec3 a, vec3 b, float ra, float rb) { float rba=rb-ra;float baba=dot(b-a,b-a);float papa=dot(p-a,p-a);float paba=dot(p-a,b-a)/baba;float x=sqrt(papa-paba*paba*baba);float cax=max(0.0,x-((paba<0.5)?ra:rb));float cay=abs(paba-0.5)-0.5;float k=rba*rba+baba;float f=clamp((rba*(x-ra)+paba*baba)/k,0.0,1.0);float cbx=x-ra-f*rba;float cby=paba-f;float s=(cbx<0.0&&cay<0.0)?-1.0:1.0;return s*sqrt(min(cax*cax+cay*cay*baba,cbx*cbx+cby*cby*baba)); }
