float d2Torus(vec3 pos,vec3 sp,vec2 t){vec3 p=pos-sp;vec2 q=vec2(length(p.xz)-t.x,p.y);return length(q)-t.y;}

