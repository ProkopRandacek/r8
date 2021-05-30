float d2CTorus(vec3 pos,vec3 sp,vec2 sc,float ra,float rb){vec3 p=pos-sp;p.x=abs(p.x);float k=(sc.y*p.x>sc.x*p.y)?dot(p.xy,sc):length(p.xy);return sqrt(dot(p,p)+ra*ra-2.0*ra*k)-rb;}

