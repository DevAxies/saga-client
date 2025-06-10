//  Renders a simple rotation cube

#ifndef ICOSPHERERENDERABLE_H
#define ICOSPHERERENDERABLE_H

#include "Renderable.h"
#include <memory>

class BasicShader;
class Mesh;

class IcoSphereRenderable : public Renderable
{
public:
    IcoSphereRenderable();
    virtual ~IcoSphereRenderable();

    void Update(float dt) override;
    void Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective) override;

    void MouseMoved(float x, float y) override { }
    void MouseClicked(float x, float y) override { }
    void ClearFocus() override { }

private:
    std::shared_ptr<Mesh> sphereMesh;
    std::shared_ptr<BasicShader> basicShader;
};

#endif