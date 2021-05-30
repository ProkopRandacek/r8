// vim: filetype=c
const char* version =
"#version ";

const char* defines =
"#define dot2(v) dot(v,v)\n"
"#define ndot(a, b) a.x*b.x-a.y*b.y\n"
// Functions to find information in shapes array
"#define sF(i, o) rawShapes[shapeSize * i + o]\n"               // return float on position `o` from shape on index `i`
"#define sV(i, o) vec3(sF(i, o), sF(i, o + 1), sF(i, o + 2))\n" //vector from 3 floats on position from o to o + 2 from shape on index `i`
"#define sT(i)  shapeTypes[i]\n"             // type
"#define sR(i)  sF(i, 9)\n"                  // radius
"#define sRs(i) sF(i, 15)\n"                 // refletiveness
"#define sP(i)  sV(i, 0)\n"                  // position
"#define sS(i)  sV(i, 6)\n"                  // scale
"#define sC(i)  vec4(sV(i, 3), sRs(i))\n"    // color
"#define sRV(i) vec2(sF(i, 9), sF(i, 10))\n" // radius vector
"#define ones vec3(1.0)\n";

const char* uniforms =
"uniform float rawShapes[shapeSize * maxShapeNum];\n"
"uniform ivec2 resolution;\n"
"uniform vec3 lightPos, cam[5];\n";

const char* consts =
"const vec3 smol = vec3(epsilon, 0.0, 0.0);\n";

const char* out =
"out vec4 outColor;\n";

const char* structs =
"struct rayHit{vec4 hitPos,surfaceClr;};\n" // hitPos.w == 0 -> didnt hit else hit
"struct map{vec4 clr;float d;};\n";

