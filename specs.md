# API Specs

WIP

## Scene creating guide

### Scene x Shader
Shader is compiled scene. Internally, it is actual GLSL code and thats why you cannot change things like group order, because the full SDF was already generated.
You can change things that are in the GLSL code represented as uniform variables.
- With shader you cannot:
  - Switch one shape type for another, nor delete or add them.
  - Change group order or group type
- With shader you can:
  - Change shape parameters
    - Color
	- Position
	- Reflectiveness
    - Radius(es) (TODO)
	- Height (TODO)
	- Scale (TODO)
	- Endpoint (TODO)
  - Change group operation modifier. (TODO)
- You can do all of this with scene

### Scene creation: 
- CreateScene(settings);
- BindScene(sceneIndex);
- \*Create shapes and groups\*
- CompileScene(sceneIndex);
- BindShader(shaderIndex);

## List of API functions

### Shape Creating
- `CreateSphere(pos, clr: ^vec3, rv, rd: real): int32`
  - Args:
    - `pos` - Position vector
	- `clr` - RGB color vector ranges 0 - 1
	- `rv`  - Reflectiveness 0 - 1
	- `rd`  - Radius, >0
  - Returns -1 if there is no more space in currently binded scene. Othrewise returns ID of the sphere
  - Example:
    - `sphere := r8.CreateSphere(&r8.vec3{0, 1.4, 0}, &r8.vec3{0, 0.4, 1}, 0, 1.2)`
- ...

### Shape Editing
- `SetShpClr(shpID: int32, clr: ^vec3)`
  - Args:
    - `shpID` - ID of the desired shape
	- `clr` - New color of the shape
  - No return value
  - Example:
    - ```
         sphere := r8.CreateSphere(pos, &r8.vec3{0, 0.4, 1}, 0, 0.2)
	 SetShpClr(sphere, &r8.vec3(1, 0, 0))
      ```
- `SetShpPos(shpID: int32, pos: ^vec3)`
  - Args:
    - `shpID` - ID of the desired shape
	- `pos` - New position of the shape
  - No return value
  - Example:
    - ```
         sphere := r8.CreateSphere(&r8.vec3{0, 0.4, 1}, clr, 0, 0.2)
	 SetShpPos(sphere, &r8.vec3(1, 0, 0))
      ```

