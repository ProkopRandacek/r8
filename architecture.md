# Architecture

## File hierarchy and short description
run `make graph` to create a visual file hierarchy from the sources
```
src
├── scene              - Code related to scenes and object in them.
│  ├── camera.c           - Create and edit camera.
│  ├── scene.c            - Create and edit scenes. Sending scene objects to GPU.
│  ├── sceneapi.c         - Fancy and safe wrappes for working with scene.
│  ├── serialization.c    - Converting scene object into arrays.
│  └── shapes.c           - Shape definitions.
├── umka               - Code related to Umka.
│  ├── bindings.c         - Binding sceneapi.c to Umka functions.
│  └── umka.c             - Umka init.
├── glslPieces/*       - Pieces of GLSL code that are joined together by shaderGen.c
├── debug.c            - Debug prints.
├── fileio.c           - Writting screenshots to disk (.bpm).
├── input.c            - Input handling. Maybe will be moved to Umka
├── main.c             - Main. Scene is created here - that is very temporary. Scene should be created in Umka
├── opengl.c           - All interactions with GL are here.
├── settings.h         - Some global settings.
├── shaderGen.c        - Compiles scene into a shader.
├── SDFGen.c           - Generates full SDF.
└── shader.c           - Compiles shader
```

## Internal scene creation process:
- CreateScene() is called from Umka
  - First free scene index is taken (`scene/sceneapi.c`)
  - Memory for the scene in scene array is allocated (`scene/scene.c`)
  - Scene settings are set (`scene/scene.c`)
  - Scene index is returned (`scene/sceneapi.c`)
- BindScene() is called from Umka
  - Active scene variable is set (`scene/sceneapi.c`)
- CompileScene() is called from Umka
- BindShader() is called from Umka

## Shapes

Shapes are defined in `shapes.h`. When serializing into a float array, following rules apply for all shapes:

- Max length of a shape is 16 floats.
Floats on positions:
```
0  1  2  is position
3  4  5  is color
6  7  8  is second position (when shape is from-to) or scale (for cube)
9        is (inner) radius (radius of a circle or radius of a smooth edge of a cube) or height
10       is (outer) radius
10 11 12 shape specific
13 14 15 shape specific
```

Shape specific rules are coded in serialization.c in {shape}2floats() functions.
