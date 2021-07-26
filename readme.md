Rewrite wip

# R8 Engine

## [Docs](./docs.md)

## TODO
- `[#...]` Docs
  - `[###.]` Design
  - `[....]` API
- `[....]` Implementation

## Download
appimage todo

## Building from source
```sh
git clone https://github.com/ProkopRandacek/r8.git
cd r8
make -j$(nproc)
./r8
```

### Deps
R8 uses [libucw](http://www.ucw.cz/libucw/) and [raylib](https://www.raylib.com/).  
You can install both of these if you have Arch: `yay -S raylib libucw --needed`. Alternatively you can download/compile and install them manually.  
The `./check` script checks that all dependencies are installed and will be found be the build process.

