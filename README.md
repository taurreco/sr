# software-rasterizer

Perspective correct interpolation.

1/z can be linearly interpolated in screen space.  So we calculate that first for our interpolated vertex, store it into Z:
```c
float Z = b0 * v0[3] + b1 * v1[3] + b2 * v2[3]; /* interpolated 1/z */
```

Then for a given attribute of that vertex, we want to find v0, v1, v2 divided by their z.  This gives us those attributes in screen space, which means we can interpolate them to get the corresponding screen space attribute for our vertex, this screen space interpolated attribute will be stored in P.  So for an attribute at index i, we have
```c
float o0 = v0[i] * v0[3];
float o1 = v1[i] * v1[3];
float o2 = v2[i] * v2[3];
float P = b0 * o0 + b1 * o1 + b2 * o2;
```
Then we can recover the homogenous attribute value (3D instead of 2D) by multiplying by its depth.  We have already interpolated the inverse of its depth in Z, so all that is left is to divide P by Z and we have the perspective correct attribute value at i.
```c
pt[i] = P / Z;
```