float opExtrusion(vec3 p,float d,float h){vec2 w=vec2(d,abs(p.z)-h);return min(max(w.x,w.y),0.0)+length(max(w,0.0));}
float sdHeart(vec2 p){p.x=abs(p.x);if(p.y+p.x>1.0)return sqrt(dot2(p-vec2(0.25,0.75)))-sqrt(2.0)/4.0;return sqrt(min(dot2(p-vec2(0.00,1.00)),dot2(p-0.5*max(p.x+p.y,0.0))))*sign(p.x-p.y);}
float d2Heart(vec3 pos,vec3 sp){return opExtrusion(pos,sdHeart(pos.xy),0.1)-0.05;}

