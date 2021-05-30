float d2Cube(vec3 pos,vec3 sp,vec3 b){vec3 p=pos-sp;vec3 q=abs(p)-b;return length(max(q,0.0))+min(max(q.x,max(q.y,q.z)),0.0);}

