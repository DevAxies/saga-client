#include "CubeRenderable.h"
#include <stdlib.h>
#include "Mesh.h"
#include "BasicShader.h"

//  Triangle vertex data
static const SimpleVertex vertexData[24] = 
{
    { -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

    { -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    
    { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    
    {  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    
    {  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    
    { -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
};

static const short indexData[36] = { 0,1,2,
                                     3,2,0,
                                     
                                     4,5,6,
                                     7,6,4,

                                     8,9,10,
                                     11,10,8,

                                     12,13,14,
                                     15,14,12,
                                     
                                     16,17,18,
                                     19,18,16,
                                     
                                     20,21,22,
                                     23,22,20 };

CubeRenderable::CubeRenderable() : yRotation(0.0f)
{
    printf("Creating CubeRenderable.\n");
    basicShader = std::make_shared<BasicShader>();
    basicMesh = Mesh::CreateMesh(VertexFormat::VERTEX_SIMPLE, (void*)vertexData, 24, (void*)indexData, 36);
}

CubeRenderable::~CubeRenderable()
{

}

void CubeRenderable::Update(float dt)
{

}

void CubeRenderable::Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective)
{
    //  Get our transform
    mat4_t model = GetWorldTransform();

    //  Render our triangle
    basicShader->BindProgram();
    basicShader->SetModel((const GLfloat*) &model);
    basicShader->SetCameraBlock(view, perspective);
    basicShader->SetEye(eye);
    
    //  Draw our mesh
    basicShader->RenderMesh(basicMesh);
}
