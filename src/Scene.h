//  This is a basic game scene

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "math_3d.h"

class Renderable;
class Accumulator;

class Scene 
{
public:
    Scene();
    ~Scene();

    void UpdateScene(float dt);
    
    //  TODO: Don't do this, the scene should know nothing about rendering
    void DrawScene(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective);

    //  Mouse interactions into the Scene, I don't love this
    void MouseMoved(float x, float y);
    void MouseClicked(float x, float y);
    void ClearFocus();

    //  Add an object to the scene
    void AddRenderable(std::shared_ptr<Renderable> toAdd);

    //  Remove a renderable 
    void RemoveRenderable(std::shared_ptr<Renderable> toRemove);

    //  Get renderable count
    int GetRenderableCount() const { return renderables.size(); }

private:
    std::vector<std::shared_ptr<Renderable>> renderables;
};

#endif