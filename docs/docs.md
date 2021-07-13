# API docs

## Data types

### Scene
Scene contains a single group and portal bridges.  
Scene is created with `scCreate(...)` which returns a scene handle for you to reference the scene later.

### Shader
Shader is compiled scene. It can't be changed. The shapes, wrappers and groups used in the scene can be changed and the changes will affect the scene.

### Wrapper
Wrapper wraps around a shape, group or another wrapper. It contains a custom GLSL code that modifies the SDF in the wrapper.

### Group
Group is a group of 2 things. Those can be shapes, other groups or wrappers. Groups can these two things:
- Connect (Union)
- Subtract (Difference)
- Intersect (Intersection)
- Blend
- Average  
- Approximate

### PortalBridge
Teleports the rays between who portal surfaces

## Functions

### Shape

#### Creates
- `shCreateSphere()`
#### Edits
- `shEditSphereRadius()`
#### Misc
- `shClone(shape)`
  - Clones a shape and returns the copy.
- `shDelete(shape)`
  - Deletes a shape.

### Group
- `grCreate(A, B, ...)`
  - Creates a group containing A and B. A and B are group or shape or a wrapper.

### Scene
- `scCreate(...)`
  - Creates a blank scene and returns it.
- `scSetGroup(scene, group)`
  - Sets a group to the scene.
- `scClone(scene)`
  - Clones a scene and returns the copy.
- `scDelete(scene)`
  - Deletes a given scene.
- `scCompile(scene)`
  - Compiles a scene and returns a shader.

### Shader
- `sdBind(shader)`
  - Render the shader to the window.

