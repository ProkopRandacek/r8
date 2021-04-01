# Architecture

## File hierarchy and short description
```
main.c                     - Code initialization, main render loop.
 |
 +-> opengl.c              - All interactions with OpenGL are here.
 |    |
 |    +-> shader.c         - Shader reading, compiling, uniforms functions.
 |
 +-> fileio.c              - Reads pixel bytes from memory and dumps them into a bmp file
 |
 +-> input.c               - Input handling
 |
 +-> vector.c              - Vector untils
 |
 +-> umka.c                - Umka initialization.
 |
 +-> debug.c               - Debug prints.
 |
 +-> fileio.c              - Writting pixel array as BMP file. Used to taking screenshots
 |
 +-> scene.c               - Sending scene to the gpu
      |
      +-> sceneapi.c       - Functions available to Umka
      |    |
      |    +-> bindings.c  - Binding the sceneapi.c to umka functions
      |
      +-> camera.c         - Calculate edge rays from camera position and facing direction. Edge rays are
      |                    - used to interpolate between, when calculating what direction to cast the rays in.
      |
      +-> shapes.c         - Definitions of shapes and shape functions.
      |
      +-> serialization.c  - Exporting structs to float / int arrays. Arrays are then send to uniforms in the shader
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
