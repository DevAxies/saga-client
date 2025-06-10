//  Basic camera implementation

#include "Camera.h"
#include "Application.h"

Camera::Camera() :
    position(vec3(0.0f, 0.0f, -5.0f)),
    lookAt(vec3(0.0f, 0.0f, 0.0f)),
    farPlane(5.001f)
{

}

Camera::~Camera()
{

}

mat4_t Camera::GetView()
{
    return m4_look_at(position, lookAt, vec3(0, 1, 0));
}

mat4_t Camera::GetPerspective()
{
    return m4_perspective(60, (float)Application::Get()->GetWidth() / (float)Application::Get()->GetHeight(), 0.01f, farPlane);
}