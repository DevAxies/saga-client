#include "IcoSphereRenderable.h"
#include <stdlib.h>
#include "Mesh.h"
#include "IcoSphere.h"
#include "BasicShader.h"

IcoSphereRenderable::IcoSphereRenderable()
{
    printf("Creating IcoSphereRenderable.\n");
    basicShader = std::make_shared<BasicShader>();
    sphereMesh = IcoSphere::Create(4);
}

IcoSphereRenderable::~IcoSphereRenderable()
{

}

void IcoSphereRenderable::Update(float dt) 
{

}

void IcoSphereRenderable::Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective)
{
    //  Get our transform
    mat4_t model = GetWorldTransform();

    //  Render our triangle
    basicShader->BindProgram();
    basicShader->SetModel((const GLfloat*) &model);
    basicShader->SetCameraBlock(view, perspective);
    basicShader->SetEye(eye);
    
    //  Draw our mesh
    basicShader->RenderMesh(sphereMesh);
}