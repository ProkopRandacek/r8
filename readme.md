# Ray marching rendering

![normal demo](https://randacek.dev/p/demo.png)  
![blending demo](https://randacek.dev/p/blending.png)  
![CSG demo](https://randacek.dev/p/csg.png)  

Current rendering capabilities:  
- [Constructive solid geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry)
- Soft shadows
- Reflections
- Sphere, Cube, Torus, Capped Cone and Cylinder rendering ([many more in testing](./src/shapes.h))
- Transforming between shapes over time
- WASD + mouse controls
- Checkerboard floor
- Works realtime

TODO:
- Umka scripting
- Modifiers for shapes (twist, round edges, displacement, ...)
- Inline functions
- Upgrade light source objects
  - Multiple lights in a scene
  - Colored lights
- Optimalizions
  - Bounding box for complicated groups far away from the ray
    - Needs render order rework
- Read scene configuration from file
  - Relative position for shapes in subgroups?
- Compile for windows somehow?
- Shape rotating
- `#import`s seems to be messy

see [`archtecture.md`](architecture.md) for explanation what every file does.  

## Demos
[Transform demo](https://randacek.dev/p/transform_demo.mp4) - (Tourus to Cube transition, rendering to file, Multiple object in scene)  
[Move demo](https://randacek.dev/p/move_demo.mp4) - (WASD & Mouse movement, Multiple object in scene)  

## Build:
`make` to (re)build the binary or `make run` to (re)build the binary and run it. Shader files are now included in the binary.
  
Compiled birary can be found [here](https://randacek.dev/p/build.7z); [checksums](https://randacek.dev/p/build.7z.checksum) (you still need glfw3 and glew)

### Runtime deps:
- glfw3  
(Arch `yay -S glfw-x11 --needed`)  
(Fedora: `dnf install glfw-devel`)  
(Ubuntu: `apt install libglfw3`)  

### Build deps:
- Runtime deps
- make
- mono (for running the shader minifier)
- gcc
- pkg-config
- unzip  
(Arch:        `yay -S gcc make unzip pkgconf    mono --needed`)  
(Fedora: `dnf install gcc make unzip pkgconf    mono-devel gcc`)  
(Ubuntu: `apt install gcc make unzip pkg-config mono-runtime gcc`)  
