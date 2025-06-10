//  This takes a path and loads a texture that cann be bound for use with rendering
//  inside OpenGL ES 3, it uses SDL to load an image

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <memory>
#include <GLFW/glfw3.h>

class Texture2D
{
    public:
        Texture2D();
        ~Texture2D();

        bool IsReady() { return isValid; }
        void BindTexture();

        static std::shared_ptr<Texture2D> CreateTexture(const char* path);

    private:
        GLuint texture;
        bool isValid = false;
};

#endif