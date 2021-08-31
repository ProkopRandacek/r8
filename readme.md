# R8 Engine

WIP

tags: Raymarching, OpenVR, Linux, seamless portals

## TODO
- VR support
- Windows build
- Remove gcc dependent code (nested functions)
- Fork OpenVR and add `.gitignore`
- Scripts
- Editor
- SDF optimizations, approximations, ...

## Docs
Generated doxygen website is [available online](https://rdck.dev/r8doc/).  
You can generate it yourself with `make docs`.

## Building from source for Linux
```sh
git clone --recursive --depth 1 --shallow-submodules https://github.com/ProkopRandacek/r8.git
cd r8
make run CC=gcc -j$(nproc)
```
Compile with `gcc`. The code seems to have issues with `clang` (TODO) and `tcc` doesn't support the `-M` flags. I didn't test any others.

### Build deps
- `gcc` -
- `make` -
- `sed` - coverts glsl code into a header
- `cmake` - openvr compile
- `doxygen` - if you want to generate docs

## Building from source for Windows
Fuck

### Build deps
Fuuuck

## Submodules
R8 uses:
- [raylib](https://www.raylib.com/) - math and opengl abstraction
- [umka-lang](https://github.com/vtereshkov/umka-lang) - scripting language
- [openvr](https://github.com/ValveSoftware/openvr) - VR support

All are built and linked by make.


