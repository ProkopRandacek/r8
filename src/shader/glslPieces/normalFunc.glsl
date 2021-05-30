vec3 calculateNormal(vec3 p){return normalize(vec3(mapWorldD(p+smol.xyy)-mapWorldD(p-smol.xyy),mapWorldD(p+smol.yxy)-mapWorldD(p-smol.yxy),mapWorldD(p+smol.yyx)-mapWorldD(p-smol.yyx)));}

