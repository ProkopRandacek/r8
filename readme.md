# R8 Engine

WIP

tags: Raymarching, OpenVR, Linux, seamless portals

## TODO
- Editor
- Scripts
- Windows build
- VR support
- Fork OpenVR and add `.gitignore`
- SDF optimizations, approximations, ...

## Docs
Generated doxygen website is [available online](https://rdck.dev/r8doc/).  
You can generate it yourself with `make docs`.

## Building from source for Linux
```sh
git clone --recursive --depth 1 --shallow-submodules https://github.com/ProkopRandacek/r8.git
cd r8
make run -j$(nproc)
```
Works with `gcc` and `clang`. `tcc` doesn't support the `-M` flags that is required to build.
I didn't test any others.

### Build deps
- `gcc`/`clang` -
- `make` -
- `sed` - coverts glsl code into a header
- `cmake` - openvr compile
- `doxygen` - if you want to generate docs

(`yay -S sed cmake make gcc doxygen --needed`)

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


