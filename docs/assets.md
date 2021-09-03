# Assets

All files needed by R8 are embedded into the binary as static variables because I think it is very convenient.  
  
All files in the `src/` directory with suffix `.asset` will get converted into a header file `original_name.asset.h`. (`font.ttf.asset` -> `font.ttf.asset.h`).
This header file contains a `const char original_name_asset_bytes[]` array of bytes from the original file (`font.ttf.asset` -> `font_ttf_asset_bytes`).
Second variable that is created is `const int original_name_asset_size` and contains a size of the binary file (`font.ttf.asset` -> `font_ttf_asset_size`).  
  
The embedding is done by the `utils/biem.c` utility (called from `make`).
