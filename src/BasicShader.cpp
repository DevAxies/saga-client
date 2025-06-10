//  Implementation of basic shader

#include "BasicShader.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>

#pragma pack(push, 1)
struct CameraBlock
{
    mat4_t view;
    mat4_t perspective;
};
#pragma pack(pop)

BasicShader::BasicShader() : BaseProgram("assets/shaders/basic_vs.glsl", "assets/shaders/basic_ps.glsl")
{
    //  Link the shader
    LinkProgram();

    //  Get attribute locations
    vertexPos = glGetAttribLocation(program, "vPos");
    vertexNorm = glGetAttribLocation(program, "vNorm");
    vertexColor = glGetAttribLocation(program, "vCol");

    //  Maps our vindings
    modelLocation = glGetUniformLocation(program, "model");
    eyeLocation = glGetUniformLocation(program, "eye");

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

BasicShader::~BasicShader()
{

}

void BasicShader::RenderMesh(std::shared_ptr<Mesh> mesh)
{
    if (mesh->GetFormat() == VertexFormat::VERTEX_SIMPLE)
    {
        //  Bind our vertex buffer
        mesh->BindVertexBuffer();

        //  Calculate stride
        const GLuint stride = GetFormatSize(VertexFormat::VERTEX_SIMPLE);

        // Bind vertex position attribute
        glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(vertexPos);

        // Bind vertex normal attribute
        glVertexAttribPointer(vertexNorm, 3, GL_FLOAT, GL_FALSE, stride, (const void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(vertexNorm);

        // Bind vertex color attribute
        glVertexAttribPointer(vertexColor, 4, GL_FLOAT, GL_FALSE, stride, (const void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(vertexColor);

        //  Draw our mesh
        mesh->DrawMesh();
    }
}

void BasicShader::SetModel(const GLfloat* model)
{
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);
}

void BasicShader::SetCameraBlock(const mat4_t& view, const mat4_t& perspective)
{
    CameraBlock cBlock;
    cBlock.view = view;
    cBlock.perspective = perspective;

    glBindBuffer(GL_UNIFORM_BUFFER, cameraBlock);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cBlock), &cBlock, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void BasicShader::SetEye(const vec3_t& eye)
{
    glUniform3f(eyeLocation, eye.x, eye.y, eye.z);
}