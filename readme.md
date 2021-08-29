# R8 Engine

## Docs
Generated doxygen website is [available online](https://rdck.dev/r8doc/).  
You can generate it yourself with `make docs`.

## TODO
- `[#...]` Docs
  - `[###.]` Design
  - `[....]` API
- `[#...]` Implementation

## Download
appimage todo

## Building from source
```sh
git clone --recursive https://github.com/ProkopRandacek/r8.git
cd r8
make run CC=gcc -j$(nproc)
```
Compile with `gcc`. The code seems to have issues with `clang` and `tcc` doesn't support the `-M` flags. I didn't test any others.

### Deps
R8 uses:
- [raylib](https://www.raylib.com/) - math and opengl abstraction
- [umka-lang](https://github.com/vtereshkov/umka-lang) - scripting language
- [openvr](https://github.com/ValveSoftware/openvr) - VR support

(all are compiled automatically and linked statically. Result of compiling R8 is a single portable executable)

### Build deps
You need:
- `gcc` -
- `make` -
- `cmake` - openvr compile
- `doxygen` - if you want to generate docs
- `sed` - coverts glsl code into a header
