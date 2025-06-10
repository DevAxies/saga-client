//  Renders a simple rotation cube

#ifndef CUBERENDERABLE_H
#define CUBERENDERABLE_H

#include "Renderable.h"
#include <memory>

class BasicShader;
class Mesh;

class CubeRenderable : public Renderable
{
public:
    CubeRenderable();
    virtual ~CubeRenderable();

    void Update(float dt) override;
    void Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective) override;

    void MouseMoved(float x, float y) override { }
    void MouseClicked(float x, float y) override { }
    void ClearFocus() override { }

private:
    float yRotation;
    std::shared_ptr<Mesh> basicMesh;
    std::shared_ptr<BasicShader> basicShader;
};

#endif