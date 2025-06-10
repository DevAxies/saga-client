//  This is the base program for shaders, all materials/programs should
//  derive from this class, it handles the basics of making and compiling
//  a shader.

#ifndef BASEPROGRAM_H
#define BASEPROGRAM_H

#include <memory>
#include <GLFW/glfw3.h>

#ifndef MESH_H
#include "Mesh.h"
#endif

class Shader;

class BaseProgram
{
    public:
        BaseProgram(const char* vs, const char* ps);
        virtual ~BaseProgram();

        void BindProgram();

        virtual void RenderMesh(std::shared_ptr<Mesh> mesh) = 0;

    protected:
        GLuint program;

        void LinkProgram();

    private:
        std::shared_ptr<Shader> vertexShader;
        std::shared_ptr<Shader> pixelShader;
};

#endif