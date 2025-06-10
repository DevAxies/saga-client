//  Implement our unlit shader

#ifndef UNLITSHADER_H
#define UNLITSHADER_H

#include "BaseProgram.h"
#include "math_3d.h"

class UnlitShader : public BaseProgram
{
    public:
        UnlitShader();
        ~UnlitShader();

        void RenderMesh(std::shared_ptr<Mesh> mesh) override;

        void SetModel(const GLfloat* model);
        void SetTint(const vec3_t rgb, const float a);
        void SetCameraBlock(const mat4_t& view, const mat4_t& perspective);

    private:
        //  Vertex Attributes
        GLuint vertexPos;
        GLuint vertexUV;
        GLuint vertexColor;

        //  Uniforms
        GLint modelLocation;
        GLint tintLocation;

        //  Uniform Blocks
        GLuint cameraBlock;
};

#endif