#include "Mesh.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    if (vertexBuffer > 0)
    {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }

    if (indexBuffer > 0)
    {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }
}

std::shared_ptr<Mesh> Mesh::CreateMesh(VertexFormat format, void* verts, unsigned vertCount, void* indices, unsigned indexCount)
{
    //  Create a new mesh
    auto mesh = std::make_shared<Mesh>();
    mesh->format = format;
    mesh->indexCount = indexCount;

    //  Try to generate our buffers
    glGenBuffers(1, &mesh->vertexBuffer);
    glGenBuffers(1, &mesh->indexBuffer);

    //  If we fail, return null
    if (mesh->vertexBuffer == 0 || mesh->indexBuffer == 0) return nullptr;

    //  Fill our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertCount * GetFormatSize(format), verts, GL_STATIC_DRAW);

    //  Fill our index buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(short), indices, GL_STATIC_DRAW);

    //  Unbind our buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //  return our new mesh
    return std::move(mesh);
}

void Mesh::BindVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
}

void Mesh::DrawMesh()
{
    // Draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

    //  Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}