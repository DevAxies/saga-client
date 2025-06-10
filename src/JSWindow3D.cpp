//  Implementation of a basic demo window

#include "JSWindow3D.h"
#include <stdlib.h>
#include "math_3d.h"
#include "Application.h"
#include "Scene.h"
#include "Camera.h"
#include <emscripten.h>

JSWindow3D::JSWindow3D()
{
    printf("Creating JSWindow3D.\n");
    scene = std::make_shared<Scene>();
    basicCamera = std::make_shared<Camera>();

    vec3_t camPos = basicCamera->GetPosition();
    printf("Camera Position: {%.2f, %.2f, %.2f}\n", camPos.x, camPos.y, camPos.z);
}

JSWindow3D::~JSWindow3D()
{
    
}

void JSWindow3D::Update(float dt)
{
    //  Do some basic camera updates
    if (shouldCameraRotate)
    {
        camTime += dt;
        vec3_t camPos = vec3(2.5f, 0.0f, 0.0f);
        mat4_t camRot = m4_rotation_y(camTime);
        camPos = m4_mul_pos(camRot, camPos);
        basicCamera->SetPosition(camPos);
    }

    //  Handle Zoom in / Zoom out
    vec3_t camPos = basicCamera->GetPosition();
    if (camPos.z != desiredZ)
    {
        if (camPos.z > desiredZ)
        {            
            camPos.z -= 10.0f * dt;
            if (camPos.z < desiredZ) camPos.z = desiredZ;
        }
        else
        {
            camPos.z += 10.0f * dt;
            if (camPos.z > desiredZ) camPos.z = desiredZ;
        }
        
        basicCamera->SetPosition(camPos);
        basicCamera->SetFarPlane((camPos.z * -1.0f) + 0.001f);
    }

    //  Update our scene
    scene->UpdateScene(dt);

    //  Create a basic transform
    mat4_t view = basicCamera->GetView();
    mat4_t perspective = basicCamera->GetPerspective();
    vec3_t eye = basicCamera->GetEye();

    //  Render our scene
    scene->DrawScene(eye, view, perspective);
}

void JSWindow3D::SetCameraPosition(float x, float y, float z)
{
    //  Sets the position, the look at point, and the far plane for the camera
    //  which all need to be updated when the camera is moved
    basicCamera->SetPosition({x, y, z});
    basicCamera->SetLookAt({x, y, 0.0f});
    basicCamera->SetFarPlane((z * -1.0f) + 0.001f);
}

void JSWindow3D::HandleKeyPress(int key, int action)
{
    //  Intentionally empty
}

void JSWindow3D::MouseDown(int x, int y)
{
    printf("Mouse Down - Location: {%d, %d}\n", x, y);
    mouseDown = true;
    mouseX = dragX = x;
    mouseY = dragY = y;
}

void JSWindow3D::MouseMove(int x, int y)
{
    if (mouseDown)
    {
        float deltaX = (float)(x - dragX);
        float deltaY = (float)(y - dragY);
        float distance = (deltaX * deltaX) + (deltaY * deltaY);
        if (distance > 25.0f) 
        {
            vec3_t camPos = basicCamera->GetPosition();
            camPos.x += deltaX * 0.01f;
            camPos.y += deltaY * 0.01f;
            dragX = x;
            dragY = y;
            basicCamera->SetPosition(camPos);
            basicCamera->SetLookAt(vec3(camPos.x, camPos.y, 0.0f));

            ClearFocus();
        }
    }
    else
    {
        //  Do tile highlight
        vec3_t mouseWorldPos = GetMouseWorldPos(x, y);
        scene->MouseMoved(mouseWorldPos.x, mouseWorldPos.y);
    }
}

void JSWindow3D::MouseUp(bool wasFocused)
{
    printf("Mouse Up - Was Focused: %s\n", wasFocused ? "True" : "False");
    if (wasFocused && mouseDown && dragX == mouseX && dragY == mouseY)
    {
        vec3_t mouseWorldPos = GetMouseWorldPos(mouseX, mouseY);
        scene->MouseClicked(mouseWorldPos.x, mouseWorldPos.y);
    }

    mouseDown = false;
    mouseX = mouseY = dragX = dragY = 0;
}

void JSWindow3D::MouseWheel(int delta)
{
    desiredZ += delta * -0.005f;
    if (desiredZ > -10.0f) desiredZ = -10.0f;
    else if (desiredZ < -20.0f) desiredZ = -20.0f;
}

void JSWindow3D::ClearFocus()
{
    scene->ClearFocus();
}

vec3_t JSWindow3D::GetMouseWorldPos(int x, int y)
{
    mat4_t view = basicCamera->GetView();
    mat4_t perspective = basicCamera->GetPerspective();
    mat4_t viewProj = m4_mul(perspective, view);
    mat4_t inverseProjView = m4_invert(viewProj);

    vec3_t screenPoint = vec3((2.0f * ((float)x / (float)Application::Get()->GetWidth())) - 1.0f,
                              (-2.0f * ((float)y / (float)Application::Get()->GetHeight())) + 1.0f,
                              1.0f);
                              
    return m4_mul_pos(inverseProjView, screenPoint);
}