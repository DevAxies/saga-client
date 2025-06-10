//  This is a basic 3D camera

#ifndef CAMERA_H
#define CAMERA_H

#include "math_3d.h"

class Camera 
{
    public:
        Camera();
        ~Camera();

        vec3_t GetPosition() const { return position; }
        vec3_t GetLookAt() const { return lookAt; }
        float GetFarPlane() const { return farPlane; }
        void SetPosition(const vec3_t& pos) { position = pos; }
        void SetLookAt(const vec3_t& look) { lookAt = look; }
        void SetFarPlane(const float& far) { farPlane = far; }

        mat4_t GetView();
        mat4_t GetPerspective();
        vec3_t GetEye() const { return position; }

    private:
        vec3_t position;
        vec3_t lookAt;
        float farPlane;
};


#endif