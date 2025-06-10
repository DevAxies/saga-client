//  Implement our basic shader

#ifndef BASICSHADER_H
#define BASICSHADER_H

#include "BaseProgram.h"
#include "math_3d.h"

class BasicShader : public BaseProgram
{
    public:
        BasicShader();
        ~BasicShader();

        void RenderMesh(std::shared_ptr<Mesh> mesh) override;

        void SetModel(const GLfloat* model);
        void SetCameraBlock(const mat4_t& view, const mat4_t& perspective);
        void SetEye(const vec3_t& eye);

    private:
        //  Vertex Attributes
        GLuint vertexPos;
        GLuint vertexNorm;
        GLuint vertexColor;

        //  Uniforms
        GLint modelLocation;
        GLint eyeLocation;

        //  Uniform Blocks
        GLuint cameraBlock;
};

#endif