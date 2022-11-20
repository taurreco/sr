# Software Renderer

<p align="center">
  <img src="https://user-images.githubusercontent.com/8971799/189614240-9449b3fe-372d-4796-8f32-3b13309ca629.png" />
  <img src="https://user-images.githubusercontent.com/8971799/202895324-0654cd65-fef2-4e0c-8c77-5db243d58374.png" />
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
* spot, point, & directional lighting
* clipping
* depth buffer
* color blending
* programmable shading stages
* custom vertex attributes
* obj loading
* tga image loading

### Example Usage

<p align="center">
  <img src="https://user-images.githubusercontent.com/8971799/202895241-8e513647-bbf7-421d-b77d-959a0b34184d.png" />
</p>

Here is a step by step explianation for how to use the sr library to render a triangle (following ```examples/basic_triangle.c```).


### Build


#### For Linux:
Should work out of the box, just run make and see the rules below for details

#### For Windows (MinGW)
Go into the Makefile and change line 70 (the example rules) and add ```$(MINGW_FLAGS)``` like so:
```Make
    $(CC) $(CFLAGS) $(MINGW_FLAGS) $< $(SR_SRC) -o $@ $(SDL2_FLAGS) -Isrc -lm
```
Make sure that the SDL2 .a files, headers, and binaries are in the ```C:\MinGW``` lib, include, and bin folders respectively, or change the ```MINGW_FLAGS``` variable to reflect where those folders are on your system.

### Makefile rules
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


