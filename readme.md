Im rewritting the entire thing in this branch.  
This time im starting with the documentations and then implementation.

# R8 Engine

![2x2 demos](https://randacek.dev/p/demos2x2.png)  

## TODO
- [ ] Docs

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
