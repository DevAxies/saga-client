//  This is a basic JS controlled window

#ifndef JSWINODW_H
#define JSWINODW_H

#include <memory>
#include "Window.h"
#include "math_3d.h"

class Camera;
class Scene;

class JSWindow3D : public Window
{
public:
    JSWindow3D();
    ~JSWindow3D();

    //  ---- Window Implementation ----
    void Update(float dt) override;
    void HandleKeyPress(int key, int action) override;
    void MouseDown(int x, int y) override;
    void MouseMove(int x, int y) override;
    void MouseUp(bool wasFocused) override;
    void MouseWheel(int delta) override;
    void ClearFocus() override;
    Scene* GetScene() const override { return scene.get(); }
    //  ---- Window Implementation ----

    //  Sets the cameras position, allows JS control over camera placement
    void SetCameraPosition(float x, float y, float z);

    //  This is a feature for testing, it will make the camera automatically rotate
    void SetCameraRotate(bool shouldRotate) { shouldCameraRotate = shouldRotate; }

private:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Camera> basicCamera;
    float camTime = 0.0f;
    bool shouldCameraRotate = false;
    
    bool mouseDown = false;
    int mouseX = 0, mouseY = 0;
    int dragX = 0, dragY = 0;
    float desiredZ = -15.0f;

    //  Converts the mouse position into world coords
    vec3_t GetMouseWorldPos(int x, int y);
};

#endif