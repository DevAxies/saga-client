//  A basic renderable in our scene

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "math_3d.h"

#define TO_RADIANS(x) (x*3.14159f/180.0f)

class Renderable
{
public:
    Renderable();
    virtual ~Renderable();

    virtual void Update(float dt) = 0;
    virtual void Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective) = 0;
    
    virtual void MouseMoved(float x, float y) = 0;
    virtual void MouseClicked(float x, float y) = 0;
    virtual void ClearFocus() = 0;

    void SetPosition(float x, float y, float z) { position = vec3(x, y, z); }
    void SetRotation(float degX, float degY, float degZ) { rotation = vec3(TO_RADIANS(degX), TO_RADIANS(degY), TO_RADIANS(degZ)); }
    void SetScale(float newScale) { scale = newScale; }
    mat4_t GetWorldTransform();

protected:
    vec3_t position;
    vec3_t rotation;
    float scale;
};

#endif