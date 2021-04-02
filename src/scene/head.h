const char* version =
"#version ";

const char* defines =
"#define maxShapeNum 10\n"
"#define maxGroupNum 10\n"
"#define shapeSize 16\n"
"#define groupSize 4\n"
"#define epsilon 0.02\n"
"#define dot2(v) dot(v,v)\n"
"#define ndot(a, b) a.x*b.x-a.y*b.y\n"
// Functions to find information in groups array
"#define gF(i, o) rawGroups[groupSize * i + o]\n" // returns float on position `o` from group on index `i`
"#define gA(i)  gF(i, 0)\n"                 // A shape/group index
"#define gB(i)  gF(i, 1)\n"                 // B shape/group index
"#define gOp(i) gF(i, 2)\n"                 // group operation
"#define gK(i)  float(gF(i, 3)) / 1000.0\n" // group operation modifier
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
//
"#define ones vec3(1.0)\n"
"#define checkerboard(p) mix(vec4(0.5, 0.5, 0.5, 1), vec4(0.7, 0.7, 0.7, 0), max(sign(mod(dot(floor(p), ones), 2.0)), 0.0))\n";

const char* uniforms =
"uniform int groupNum, shapeNum, rawGroups[groupSize * maxGroupNum], shapeTypes[maxShapeNum];\n"
"uniform float rawShapes[shapeSize * maxShapeNum];\n"
"uniform ivec2 resolution;\n"
"uniform vec3 lightPos, cam[5];\n";

const char* globals =
"vec4  d2GroupsC[maxGroupNum];\n" // colors
"float d2GroupsD[maxGroupNum], d2Shapes[maxShapeNum];\n";

const char* consts =
"const vec3 smol = vec3(epsilon, 0.0, 0.0);\n";

const char* out =
"out vec4 outColor;\n";

const char* structs =
"struct rayHit{vec4 hitPos,surfaceClr;};\n" // hitPos.w == 0 -> didnt hit else hit
"struct map{vec4 clr;float d;};\n";
