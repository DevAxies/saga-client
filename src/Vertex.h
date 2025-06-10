//  Hewlpers for vertex formats

#ifndef VERTEX_H
#define VERTEX_H

enum class VertexFormat : int 
{
    VERTEX_NONE = 0,
    VERTEX_SIMPLE = 1,
    VERTEX_UNLIT = 2
};

#pragma pack(push, 1)

struct SimpleVertex
{
    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
};

struct UnlitVertex
{
    float x, y, z;
    float u, v;
    float r, g, b, a;
};

#pragma pack(pop)

unsigned GetFormatSize(VertexFormat format);

#endif