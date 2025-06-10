//  Implementation of the shader class

#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector> 
#include <GLES3/gl3.h>
#include <EGL/egl.h>

Shader::Shader()
{

}

Shader::~Shader()
{
    glDeleteShader(loadedShader);
}

void Shader::Init(const char* fileName, GLuint shaderType)
{
    //  Open our file
    FILE *file = fopen(fileName, "rb");
    if (!file) 
    {
        printf("Cannot open file \"%s\"\n", fileName);
        return;
    }

    //  Read the contents
    std::stringstream ss;
    while (!feof(file)) 
    {
        char c = fgetc(file);
        if (c != EOF) 
        {
            ss << c;
        }
    }
    fclose (file);

    //  Save it off to our internal data structure
    shaderData = ss.str();

    //  Create our loaded shader
    const char* shaderStr = shaderData.c_str();
    loadedShader = glCreateShader(shaderType);
    glShaderSource(loadedShader, 1, &shaderStr, NULL);
    glCompileShader(loadedShader);

    //  Verify the shader compiled
    GLint isCompiled = 0;
    glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(loadedShader, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(loadedShader);

        //  Print the log
        printf("%s\n", &infoLog[0]);
    }
}

std::shared_ptr<Shader> Shader::CreateVertexShader(const char* filePath)
{
    auto out = std::make_shared<Shader>();
    out->Init(filePath, GL_VERTEX_SHADER);
    return std::move(out);
}

std::shared_ptr<Shader> Shader::CreatePixelShader(const char* filePath)
{
    auto out = std::make_shared<Shader>();
    out->Init(filePath, GL_FRAGMENT_SHADER);
    return std::move(out);
}