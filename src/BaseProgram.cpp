//  Implement our BaseProgram

#include "BaseProgram.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Shader.h"

BaseProgram::BaseProgram(const char* vs, const char* ps)
{
    vertexShader = Shader::CreateVertexShader(vs);
    pixelShader = Shader::CreatePixelShader(ps);
    
    //  Create a program, yell if it isn't valid
    program = glCreateProgram();
    if (program == 0)
    {
        printf("Failed to create a Shader Progran.\n");
        return;
    }
    
    //  Attach our shaders
    glAttachShader(program, vertexShader->GetShader());
    glAttachShader(program, pixelShader->GetShader());
}

BaseProgram::~BaseProgram()
{

}

void BaseProgram::LinkProgram()
{
    //  Link our program
    glLinkProgram(program);
    
    // Check the link status
    GLint linked; 
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) 
    {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            printf("Error linking program:\n%s\n", infoLog);            
            free(infoLog);
        }

        glDeleteProgram(program);
        return;
    }
}

void BaseProgram::BindProgram()
{
    glUseProgram(program);
}