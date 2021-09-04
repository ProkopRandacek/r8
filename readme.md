# R8 Engine

WIP

## TODO
- Editor
- Scripts
- Windows build
- VR support
- SDF optimizations, approximations, ...

## Docs
Generated doxygen website is [available online](https://rdck.dev/r8doc/).  
You can generate it yourself with `make docs`.

## Building on Linux for Linux
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

## Building on Linux for Windows

## Submodules
R8 uses:
- [raylib](https://www.raylib.com/) - math and opengl abstraction
- [umka-lang](https://github.com/vtereshkov/umka-lang) - scripting language
- [openvr](https://github.com/ValveSoftware/openvr) - VR support

All are built and linked by make.


