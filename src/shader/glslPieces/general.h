// vim: filetype=c
#ifndef GENERAL_H
#define GENERAL_H

const char* vertSource = "layout(location=0)in vec3 aPos;void main(){gl_Position=vec4(aPos,1.);}\n";
const char* normalFunc = "vec3 calculateNormal(vec3 p){return normalize(vec3(mapWorldD(p+smol.xyy)-mapWorldD(p-smol.xyy),mapWorldD(p+smol.yxy)-mapWorldD(p-smol.yxy),mapWorldD(p+smol.yyx)-mapWorldD(p-smol.yyx)));}\n";
const char* intersectionFunc = "bool intersection(vec3 ro,vec3 rd){const vec3 box=vec3(4,3,4);vec3 tMin=(-box-ro)/rd;vec3 tMax=(box-ro)/rd;vec3 t1=min(tMin,tMax);vec3 t2=max(tMin,tMax);return max(max(t1.x,t1.y),t1.z)<=min(min(t2.x,t2.y),t2.z);}\n";
const char* checkerboard = "vec4 checkerboard(vec3 p){return mix(vec4(0.5,0.5,0.5,1),vec4(0.7,0.7,0.7,0),max(sign(mod(dot(floor(p),ones),2.0)),0.0))}\n";

const char* combineFuncs =
"map maxM(map a, map b) {\n"
"        if (a.d < b.d) return b;\n"
"        return a;\n"
"}\n"

"map minM(map a, map b) {\n"
"        if (a.d > b.d) return b;\n"
"        return a;\n"
"}\n"

"map cutM(map a, map b) {\n"
"        b.d = -b.d;\n"
"        if (a.d < b.d) return b;\n"
"        return a;\n"
"}\n";

const char* mapFuncStart =
"map mapWorld(vec3 pos) {\n"
"        return ";
const char* mapFuncEnd =
";\n"
"}\n"
"float mapWorldD(vec3 pos) { return mapWorld(pos).d; }\n";

const char* mainFunc =
"void main() {\n"
         // calculate ray direction
"        vec2 uv = gl_FragCoord.xy / resolution.xy;\n"

"        vec3 dir = normalize(mix(mix(cam[3], cam[1], uv.y), mix(cam[4], cam[2], uv.y), uv.x) - cam[0]);\n"
"        vec3 pos = cam[0];\n"

         // test if the ray is gonna intersect with the world box, if not, just render it as a sky box
"        if (!intersection(pos, dir)) {\n"
"                outColor = vec4(0.12, 0.12, 0.12, 1);\n"
"        } else {\n"
"                vec3 finalClr;\n"
"                float lastR;\n"
"                rayHit hit;\n"
                 // cast main ray
"                for (int i = 0; i < BOUNCES + 1; i++) {\n"
"                        hit = rayMarch(pos, dir);\n"

"                        finalClr = mix(hit.surfaceClr.rgb, mix(finalClr, hit.surfaceClr.rgb, lastR), min(i, 1));\n"

"                        if (hit.hitPos.w == 0.) break; // only reflect and shadown when hit a surface\n"

"                        lastR = hit.surfaceClr.w;\n"

"                        vec3 normal = calculateNormal(hit.hitPos.xyz);\n"
"                        vec3 smolNormal = normal * BACK_STEP;\n"

"                        pos = hit.hitPos.xyz + smolNormal;\n"
"                        dir = reflect(dir, normal);\n"

"                        if (hit.surfaceClr.w == 0.) {\n" // shadow only 100% non reflective surfaces. shadows on reflective surfaces look weird.
"                                finalClr *= rayMarchShadow(pos, normalize(lightPos - pos), length(lightPos - pos));\n"
"                                break;\n"
"                        }\n"
"                }\n"

                 // output color
"                outColor = vec4(finalClr, 1.);\n"
"        }\n"
"}\n";
#endif
