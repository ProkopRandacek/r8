# R8 Engine

## [Docs](./docs.md)

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
make run -j$(nproc)
```
Compile with `gcc`. The code seems to have issues with `clang` and `tcc` doesn't support the `-M` flags.

### Deps
R8 uses [libucw](http://www.ucw.cz/libucw/) and [raylib](https://www.raylib.com/). Both are included and linked statically.

