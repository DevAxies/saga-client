#include "Texture2D.h"
#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>


Texture2D::Texture2D()
{
}

Texture2D::~Texture2D() 
{
    glDeleteTextures(1, &texture);
}

std::shared_ptr<Texture2D> Texture2D::CreateTexture(const char* path)
{
    SDL_Surface *image;
    if(!(image = IMG_Load(path))) {
        fprintf(stderr, "Could not load texture image: %s\n", IMG_GetError());
        return nullptr;
    }

    auto texture2D = std::make_shared<Texture2D>();

    glGenTextures(1, &texture2D->texture);
    glBindTexture(GL_TEXTURE_2D, texture2D->texture);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);

    return std::move(texture2D);
}

void Texture2D::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}