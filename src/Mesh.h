//  Provides a basic wrapper for rendering a mesh, takes a Vertex format for binding the vertex attributes

#ifndef MESH_H
#define MESH_H

#ifndef VERTEX_H
#include "Vertex.h"
#endif

#include <memory>
#include <GLFW/glfw3.h>

class Mesh 
{
    public:
        Mesh();
        ~Mesh();

        VertexFormat GetFormat() { return format; }
        void BindVertexBuffer();
        void DrawMesh();

        static std::shared_ptr<Mesh> CreateMesh(VertexFormat format, void* verts, unsigned vertCount, void* indices, unsigned indexCount);

    private:
        VertexFormat format;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        GLuint indexCount;
};

#endif