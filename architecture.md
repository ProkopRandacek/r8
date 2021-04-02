# Architecture

## File hierarchy and short description
```
src
├── scene              - Code related to scenes and object in them
│  ├── camera.c        - Calculate edge rays from camera position and facing direction. Edge rays are
│  │                   - used to interpolate between, when calculating what direction to cast the rays in.
│  ├── scene.c         - Main scene handling
│  ├── sceneapi.c      - Functions available to Umka
│  ├── serialization.c - Exporting structs to float / int arrays. Arrays are then send to uniforms in the shader
│  ├── shaderGen.c     - Generates shader source from scene settings
│  ├── shapes.c        - Definitions of shapes and shape functions.
│  ├── SDFs.h          - GLSL SDF functions
│  ├── general.h       - General GLSL code
│  ├── head.h          - GLSL header code
│  └── rayMarch.h      - GLSL ray machring code
├── umka               - Umka related code
│  ├── bindings.c      - Binding sceneapi.c to Umka functions
│  └── umka.c          - Umka init
├── debug.c            - Debug prints
├── fileio.c           - Takes pixel array and writes it as a BMP file to disk. Used as a screenshot
├── input.c            - Input handling
├── main.c             - Main
├── opengl.c           - All interactions with GL are here
├── settings.h         - Some global settings
└── shader.c           - Shader compiling
```

## Shapes

Shapes are defined in `shapes.h`. When serializing into a float array, following rules apply for all shapes:

- Max length of a shape is 16 floats.
Floats on positions:
```
0  1  2  is position
3  4  5  is color
6  7  8  is second position (when shape is from-to) or scale
9        is (inner) radius (radius of a circle or radius of a smooth edge of a cube) or height
10       is (outer) radius
10 11 12 shape specific
13 14 15 shape specific
```

Shape specific rules are coded in serialization.c in {shape}2floats() functions.
