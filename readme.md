# R8 Engine

![2x2 demos](https://randacek.dev/p/demos2x2.png)  

## Done
- [Constructive solid geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry)
- Soft shadows
- Reflections
- Sphere, Cube, Torus, Capped Cone and Cylinder rendering (I will add more as I need them)
- Blend, average, mask, cut and union shape operations
- WASD + mouse controls
- Scene modeling from [Umka](https://github.com/vtereshkov/umka-lang)
- Works real time
- Works on Linux and Windows

## TODO
- [Umka](https://github.com/vtereshkov/umka-lang) scripting
  - Shape editing
  - Player input
- Modifiers for shapes (twist, round edges, displacement, ...)
- Inline functions (there are probably some funcs that should be inlined)
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

## Build
`make` to (re)build the binary or `make run` to (re)build the binary and run it.  
Compiled binary can be found [here](https://randacek.dev/p/build.7z); [checksums](https://randacek.dev/p/build.7z.checksum)

### Runtime deps
- None! All sources are downloaded, compiled and included in the binary automatically when building ^^

### Build deps
- make
- cmake (for glfw build, Linux build only)
- gcc
- unzip
- libx{randr, inerama, cursor, i} (Linux build only)
- mingw for compiling for windows (run as `make wbuild -e MINGW=mingw-gcc-binary-name-on-your-system`)  
Arch:  
         `yay -S gcc make cmake unzip libxrandr       libxinerama       libxcursor       libxi --needed`  
Fedora:  
`dnf     install gcc make cmake unzip libxrandr-devel libxinerama-devel libxcursor-devel libxi-devel`  
Ubuntu:  
`apt-get install gcc make cmake unzip libxrandr-dev   libxinerama-dev   libxcursor-dev   libxi-dev`  
