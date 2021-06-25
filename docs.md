# API docs

## Data types

### Scene
Scene contains a single group and as many models as you want.  
Scene is created with `scCreate(...)` which returns a scene handle for you to reference the scene later.

### Shader
Shader is compiled scene. It can't be changed. The shapes, wrappers and groups used in the scene can be changed and the changes will affect the scene.

### Wrapper
Wrapper wraps around a shape, group or another wrapper. It contains a custom GLSL code that modifies the SDF in the wrapper.

### Model
Model is supposed to be a complicated group that is expensive to calculate.  
Models have a 2 SDFs. First SDF is the real SDF that describes the model. The second one is just a simple SDF that is very fast to evaluate and is used if the ray is far enough to approximate the complicated SDF.

### Group
Group is a group of 2 things. Those can be shapes, other groups or wrappers. Groups can these two things:
- Connect (Union)
- Subtract (Difference)
- Intersect (Intersection)
- Blend
- Average  
Groups are the implementation of [CSG](https://en.wikipedia.org/wiki/Constructive_solid_geometry)

### PortalBridge

## Functions

### Shape

#### Creates
- `shCreateSphere()`
#### Edits
- `shEditSphereRadius()`
#### Misc
- `shClone(shape)`
  - Clones a model and returns the copy.
- `shDelete(shape)`
  - Deletes a shape.

### Group
- `grCreate(A, B, ...)`
  - Creates a group containing A and B. A and B are group or shape or a wrapper.

### Model
- `mdCreate(...)`
  - Creates a new model and returns it.
- `mdSetGroup(model, shape)`
  - Adds a group to the model.
- `mdClone(model)`
  - Clones a model and returns the copy.
- `mdDelete(model)`
  - Deletes a given model.

### Scene
- `scCreate(...)`
  - Creates a blank scene and returns it.
- `scSetGroup(scene, group)`
  - Sets a group to the scene.
- `scAddModel(scene, model)`
  - Adds a model to the scene. The number of models in scene is unlimited.
- `scRemoveModel(scene, model)`
  - Removes a model from a scene.
- `scClone(scene)`
  - Clones a scene and returns the copy.
- `scDelete(scene)`
  - Deletes a given scene.
- `scCompile(scene)`
  - Compiles a scene and returns a shader.

### Shader
- `sdBind(shader)`
  - Render the shader to the window.
