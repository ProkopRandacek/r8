float d2TriPrism(vec3 pos,vec3 sp,vec2 h){vec3 p=pos-sp;vec3 q=abs(p);return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);}

