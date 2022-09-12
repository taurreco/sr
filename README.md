# Software Renderer

<p align="center">
  <img src="https://user-images.githubusercontent.com/8971799/189614240-9449b3fe-372d-4796-8f32-3b13309ca629.png" />
</p>

### About
SR is a lightweight software rendering library written in C.  It exposes the main algorithms, data structures, and math behind the standard real-time rendering model used in popular graphics api's like OpenGL and DirectX3D.  This project is entirely educational, and is free for anyone to use / adapt.  SR is tested with the [Unity Framework](http://www.throwtheswitch.org/unity) and the examples use SDL2 in order to display its output buffer.

### Features
<p align="right">
  <img align="right" src="https://user-images.githubusercontent.com/8971799/189619522-cc23a50b-4dfd-4391-95c2-3a930e215c3d.png" />
</p>
<p align="right">
  <img align="right" src="https://user-images.githubusercontent.com/8971799/189634455-dd1d293c-930b-4c03-9341-2c6949d07b8e.png" />
</p>

* perspective-correct interpolation
* texture mapping
* matrix stack
* phong lighting
* clipping
* depth buffer
* color blending
* custom shaders
* custom vertex attributes
* obj loading
* tga image loading

### Design Overview
The core of the library is written in `sr_pipe.c`, where the rendering pipeline is implemented.  Its functionality depends on data organized into a struct called `sr_pipeline`:
```c
/* sr.h */

struct sr_pipeline {
    
    struct sr_framebuffer* fbuf;
    
    void* uniform;        
    vs_f vs;
    fs_f fs;

    float* pts_in;
    int n_pts;
    int n_attr_in;
    int n_attr_out;

    int winding;
};
```
The framebuffer structure `fbuf` serves primarily as the product of an `sr_render` call.  Contained within it is an image buffer that the render function fills.  

The next three fields, `uniform`, `vs`, and `fs` are the programmable parts of the rendering pipeline.  `vs` and `fs` are the vertex shader and fragment shader function pointers, respectivley.  Each one takes a reference to the `uniform`, which stores all data used by the shader that does not vary per vertex while rendering a mesh like textures, materials, bump maps, etc.

The next four fields are data specific to the model.
SR uses one float buffer called `pts` to store the vertex attribute data for rendering.  Each vertex is laid out contiguously in the buffer, where the stride of each vertex is given by `n_attr` (number of attributes).  For example, for a vertex type with a position and color attribute, the pts buffer might look like this:
```c
float pts[2 * 6] = {
    /* x, y, z */       /* r, g, b */
    0, 1, 2,            0.3, 0.1, 0.0,
    1, -1, 0,           0.9, 0.3, 0.4
};
                        
```
This buffer has two points, each with six attributes.  As such, `n_pts = 2` and `n_attr = 6`.  This way most every kind of vertex attribute layout desired by the user can be used.  In the pipeline, `pts_in` holds all the vertices of an object in model space.  And `n_attr_out` specifies how the vertex shader changes the attribute layout so that space can be pre-allocated for it.  The user will be responsible for how the access the memory in the input and output vertex buffers from the vertex shader-- the only garuntee is that they will have the memory available.

Finally, `winding` specefies a winding order of the input vertices: 1 for counter-clock-wise and -1 for clock-wise.

The only assumptions SR will make about the user defined vertex shader is that the clip space coordinates of the vertex (x, y, z, w) appear at the front of the buffer.

<p align="center">
  <img src="https://user-images.githubusercontent.com/8971799/189635638-c78c674f-316b-4337-b5c3-75948d0e468e.png" />
  <img src="https://user-images.githubusercontent.com/8971799/189635641-df877cd6-5575-4f24-8a3d-e2ae5e21b405.png" />
</p>

On top of this core, there exists a fixed function pipeline with a more accessable api supplied in `sr_lib.c`.  The library keeps a global pipeline context and custom uniform type `sr_uniform`.  Functions in the library allow for interaction with these global state variables easily.  

To give control over the model view projection transform, the user can switch between matrix 'modes' using `sr_matrix_mode`.  The modes correspond to either the model, view, or projection matrix.  Then the user can make transformations using `sr_translate`, `sr_scale`, etc.  Or make a view matrix with `sr_look_at`.  This approach is drawn from early implementations of OpenGL.

The library also supplies custom lighting for up to eight lights.  Within the uniform is an array of lights whose fields can be set by the `sr_light` function.

### Build
For linux:

to make the examples go into the repository and type 
```
$ make examples
```
then enter the examples directory
```
$ cd examples
```
and then run the example you desire.

To make the tests go into the repository and type
```
$ make tests
```
then enter the tests directory
```
$ cd tests
```
and then run the test you deire.

To clean tests executables run
```
$ make clean-tests
```
And likewise to clean the example executables run 
```
$ make clean-examples
```

### Credits

Happy Mask Salesman model & texture do not belong to me

Link model & texture created by [ps1guy](https://sketchfab.com/ps1guy) on sketchfab

Thanks to the [Graphics Programming Discord](https://discord.com/invite/6mgNGk7) for helping me along the way!


