# software-rasterizer

Perspective correct interpolation.

1/z can be linearly interpolated in screen space.  So we calculate that first for our interpolated vertex, store it into Z:
```c
float Z = b0 * v0[3] + b1 * v1[3] + b2 * v2[3]; /* interpolated 1/z */
```

For a given attribute in our vertex at index i, we want to interpolate correspinding attributes in v0, v1, v2 with screen space barycentric weights to find it.  But for now we have every attribute value in homogeneous space.  Our barycentric weights are calbirated for screen space coordinates, so we have to convert the attribute values in v0, v1, v2 to screen space by dividing by their z.  Store these in omega values, where omega_n represents the screen space attribute at triangle vertex_n.  
```c
float o0 = v0[i] * v0[3];
float o1 = v1[i] * v1[3];
float o2 = v2[i] * v2[3];
```
Then we can use screen space barycentric weights to interpolate and find the screen space attribite value, P, of our interpolated vertex.
```c
float P = b0 * o0 + b1 * o1 + b2 * o2;
```
Then we can recover the homogeneous attribute value by multiplying by its z.  We have already interpolated the inverse of its depth in Z, so all that is left is to divide P by Z and we have the perspective correct attribute value at i.
```c
pt[i] = P / Z;
```