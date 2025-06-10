//  Implementation of unlit shader

#include "UnlitShader.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>

#pragma pack(push, 1)
struct CameraBlock
{
    mat4_t view;
    mat4_t perspective;
};
#pragma pack(pop)

UnlitShader::UnlitShader() : BaseProgram("assets/shaders/unlit_vs.glsl", "assets/shaders/unlit_ps.glsl")
{
    //  Link the shader
    LinkProgram();

    //  Get attribute locations
    vertexPos = glGetAttribLocation(program, "vPos");
    vertexUV = glGetAttribLocation(program, "vUv");
    vertexColor = glGetAttribLocation(program, "vCol");

    //  Maps our vindings
    modelLocation = glGetUniformLocation(program, "model");
    tintLocation = glGetUniformLocation(program, "tint");

    // the binding point must be smaller than GL_MAX_UNIFORM_BUFFER_BINDINGS
    GLuint bindingPoint = 1, blockIndex;
    CameraBlock cBlock;
    cBlock.view = m4_identity();
    cBlock.perspective = m4_identity();
 
    blockIndex = glGetUniformBlockIndex(program, "CameraBlock");
    glUniformBlockBinding(program, blockIndex, bindingPoint);
 
    glGenBuffers(1, &cameraBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraBlock);
 
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cBlock), &cBlock, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, cameraBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UnlitShader::~UnlitShader()
{

}

void UnlitShader::RenderMesh(std::shared_ptr<Mesh> mesh)
{
    if (mesh->GetFormat() == VertexFormat::VERTEX_UNLIT)
    {
        //  Bind our vertex buffer
        mesh->BindVertexBuffer();

        //  Calculate stride
        const GLuint stride = GetFormatSize(VertexFormat::VERTEX_UNLIT);

        // Bind vertex position attribute
        glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(vertexPos);

        // Bind vertex normal attribute
        glVertexAttribPointer(vertexUV, 2, GL_FLOAT, GL_FALSE, stride, (const void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(vertexUV);

        // Bind vertex color attribute
        glVertexAttribPointer(vertexColor, 4, GL_FLOAT, GL_FALSE, stride, (const void *) (5 * sizeof(float)));
        glEnableVertexAttribArray(vertexColor);

        //  Draw our mesh
        mesh->DrawMesh();
    }
}

void UnlitShader::SetModel(const GLfloat* model)
{
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);
}

void UnlitShader::SetTint(const vec3_t rgb, const float a)
{
    glUniform4f(tintLocation, rgb.x, rgb.y, rgb.z, a);
}

void UnlitShader::SetCameraBlock(const mat4_t& view, const mat4_t& perspective)
{
    CameraBlock cBlock;
    cBlock.view = view;
    cBlock.perspective = perspective;

    glBindBuffer(GL_UNIFORM_BUFFER, cameraBlock);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cBlock), &cBlock, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}