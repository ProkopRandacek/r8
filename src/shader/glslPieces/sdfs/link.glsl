float d2Link(vec3 pos,vec3 sp,float le,float r1,float r2){vec3 p=pos-sp;vec3 q=vec3(p.x,max(abs(p.y)-le,0.0),p.z);return length(vec2(length(q.xy)-r1,q.z))-r2;}

