vec4 checkerboard(vec3 p){return mix(vec4(0.5,0.5,0.5,1),vec4(0.7,0.7,0.7,0),max(sign(mod(dot(floor(p),ones),2.0)),0.0))}

