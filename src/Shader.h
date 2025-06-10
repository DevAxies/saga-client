//  This class wraps up the loading and data handling for a shader
//  it doesn't create any actual programs, that is what a material
//  is for.

#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>
#include <GLFW/glfw3.h>

class Shader
{
    public:
        Shader();
        ~Shader();

        void Init(const char* fileName, GLuint shaderType);

        const char* GetData() const { return shaderData.c_str(); }
        const GLuint GetShader() const { return loadedShader; }

        static std::shared_ptr<Shader> CreateVertexShader(const char* filePath);
        static std::shared_ptr<Shader> CreatePixelShader(const char* filePath);

    private:
        std::string shaderData; 
        GLuint loadedShader;
};

#endif