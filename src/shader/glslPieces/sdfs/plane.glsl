float d2Plane(vec3 pos,vec3 sp,vec3 n,float h){vec3 p=pos-sp;return dot(p,normalize(n))+h;}

