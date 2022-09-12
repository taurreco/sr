# Software Renderer

<p align="center">
  <img src="https://user-images.githubusercontent.com/8971799/189614240-9449b3fe-372d-4796-8f32-3b13309ca629.png" />
</p>

### About
SR is a lightweight software rendering library written in C.  It provides a core programmable pipeline in `sr_pipe.c`, and then builds on top of that a much easier to use rendering api in `sr_lib.c`.  This project is entirely educational, and is free for anyone to use / adapt.

### Features
<p align="right">
  <img align="right" src="https://user-images.githubusercontent.com/8971799/189619522-cc23a50b-4dfd-4391-95c2-3a930e215c3d.png" />
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
/* in sr.h */

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


### Build
