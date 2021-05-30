float d2HexPrism(vec3 pos,vec3 sp,vec2 h){vec3 p=pos-sp;const vec3 k=vec3(-0.8660254,0.5,0.57735);p=abs(p);p.xy-=2.0*min(dot(k.xy,p.xy),0.0)*k.xy;vec2 d=vec2(length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x),h.x))*sign(p.y-h.x),p.z-h.y);return min(max(d.x,d.y),0.0)+length(max(d,0.0));}

