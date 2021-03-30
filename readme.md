# R8 Engine

![2x2 demos](https://randacek.dev/p/demos2x2.png)  

## Done:
- [Constructive solid geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry)
- Soft shadows
- Reflections
- Sphere, Cube, Torus, Capped Cone and Cylinder rendering (I will add more as I need them)
- Blend, average, mask, cut and union shape operations
- WASD + mouse controls
- Scene modeling from [Umka](https://github.com/vtereshkov/umka-lang)
- Works real time
- Works on Linux and Windows

## TODO:
- [Umka](https://github.com/vtereshkov/umka-lang) scripting
  - Shape editing
  - Player input
- Merge `sceneapi.c` and `shapes.c` constructors
- Update architecture.md
- Modifiers for shapes (twist, round edges, displacement, ...)
- Inline functions (there are probably some funcs that should be inlined)
- Reflectiveness for non integer values looks broken.
- Upgrade light source objects
  - Multiple lights in a scene
  - Colored lights
- Optimization
  - Bounding box for complicated groups far away from the ray
    - Needs render order rework
- Read scene configuration from file?
  - Relative position for shapes in subgroups?
- Shape rotating
- `#import`s seems to be messy

see [`archtecture.md`](architecture.md) for explanation what every file does.  

## Demos
[Transform demo](https://randacek.dev/p/transform_demo.mp4) - (Torus to Cube transition, rendering to file, Multiple object in scene)  
[Move demo](https://randacek.dev/p/move_demo.mp4) - (WASD & Mouse movement, Multiple object in scene)  

## Build:
`make` to (re)build the binary or `make run` to (re)build the binary and run it.  
Compiled birary can be found [here](https://randacek.dev/p/build.7z); [checksums](https://randacek.dev/p/build.7z.checksum)

### Runtime deps:
- None! All sources are downloaded, compiled and included in the binary automatically when building ^^

### Build deps:
- make
- cmake (for glfw build, Linux build only)
- mono (for running the shader minifier)
- gcc
- unzip
- libx{randr, inerama, cursor, i} (Linux build only)
- mingw for compiling for windows (run as `make wbuild -e MINGW=mingw-gcc-binary-name-on-your-system`)  
Arch:  
         `yay -S gcc make cmake unzip libxrandr       libxinerama       libxcursor       libxi       mono --needed`  
Fedora:  
`dnf     install gcc make cmake unzip libxrandr-devel libxinerama-devel libxcursor-devel libxi-devel mono-core`  
Ubuntu:  
`apt-get install gcc make cmake unzip libxrandr-dev   libxinerama-dev   libxcursor-dev   libxi-dev   mono-runtime`  
