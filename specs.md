# API Specs

WIP

## Scene creating guide



### Scene edit ability
Scene is internally a shader. When you compile it, it is compiled with OpenGL and you can make changes that would require to edit the shader code.
- After compiling you cannot:
  - Switch one shape type for another, nor delete or add them.
  - Change group order or group type
- But can:
  - Change shape parameters
    - Color
	- Position
	- Reflectiveness
    - Radius(es) (TODO)
	- Height (TODO)
	- Scale (TODO)
	- Endpoint (TODO)
  - Change group operation modifier. (TODO)
- Before compiling you can do all of these

### Scene creation: 
- CreateScene(settings); // Creates a blank scene
- EditScene(sceneIndex); // This specifies what scene you want to edit with the following api calls
- // Modify the scene
- CompileScene(sceneIndex);
- // You cant modify the scene anymore
- BindScene(sceneIndex);
- // The scene is being rendered

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

