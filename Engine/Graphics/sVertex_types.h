#pragma once

// These structures match the PLY file format
struct sVertex_XYZW_RGBA_N_UV_T_B
{
    float x, y, z, w;   
    float r, g, b, a;   
    float nx, ny, nz, nw;  
    float u0, v0, u1, v1;  
    float tx, ty, tz, tw;   
    float bx, by, bz, bw;   
};