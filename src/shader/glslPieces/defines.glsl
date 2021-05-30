#define dot2(v) dot(v,v)
#define ndot(a, b) a.x*b.x-a.y*b.y
// Functions to find information in shapes array
#define sF(i, o) rawShapes[shapeSize * i + o]               // return float on position `o` from shape on index `i`
#define sV(i, o) vec3(sF(i, o), sF(i, o + 1), sF(i, o + 2)) //vector from 3 floats on position from o to o + 2 from shape on index `i`
#define sT(i)  shapeTypes[i]             // type
#define sR(i)  sF(i, 9)                  // radius
#define sRs(i) sF(i, 15)                 // refletiveness
#define sP(i)  sV(i, 0)                  // position
#define sS(i)  sV(i, 6)                  // scale
#define sC(i)  vec4(sV(i, 3), sRs(i))    // color
#define sRV(i) vec2(sF(i, 9), sF(i, 10)) // radius vector
#define ones vec3(1.0)

