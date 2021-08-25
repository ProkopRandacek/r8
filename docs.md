# Design

## Scene definition
Scene is defined as a tree of shapes.

### Shape
The term shape can describe either a group, wrapper or a primitive.

### Container
The term container can describe eiter a group or a wrapper.

### Group
Group always holds two shapes. Group can modify how the two shapes it holds are combined.  
Some groups require a single float `k` to configure their behavior.  
There are 6 group types:

Group type   | Description
-------------|-------------
Union        | Connects both shapes without changing
Difference   | The second shape is subtracted from the first one
Intersection | The final shape exists only on intersection of both sub-shapes
Blend        | The two shapes are blend together. `k` defines blend strength
Average      | Linearly interpolates between the two sub-shapes, `k` is in range 0 - 1. 0 means only first shape, 1 means only second.
Approximate  | Returns the second shape. Only if second shape returns value less than `k`, is the first shape is evaluated and returned instead.

### Wrapper
Wrapper holds a single shape. Wrapper can modify the shape it holds.  

Wrapper type | Description
-------------|-------------
TODO         | TODO

### Primitive
Primitives are primitive shapes that R8 can render natively. Any more complex shapes are defined out of these using groups and wrappers. Each primitive has got a single color. There are no textures.
There are 4 primitives: (for now)

Primitive type | Description
---------------|-------------
Sphere         | Sphere has got a center point and a radius.
Cube           | Cube has got a center point and a single 3D vector that defines its scale.
Torus          | Torus has got a center point and a 2D vector, that defines the distance from the center of the tube to the center of the torus and the radius of the tube.
Capsule        | Capsule has got a start and an end point and a radius.

# Implementation

## Primitives serialization

Primitives are all serialized into array of 12 floats. Not all floats are always used.  
[Primitives serialization and glsl functions lookup table](https://docs.google.com/spreadsheets/d/1u5eLoQ5e--L0BYsAkOEX2-lQDHC-FDW7x1jXfpOxG8Y/edit?usp=sharing)

