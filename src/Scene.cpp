#include "Scene.h"
#include "Renderable.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::UpdateScene(float dt)
{
    for (unsigned i = 0; i < renderables.size(); ++i)
    {
        renderables[i]->Update(dt);
    }
}
        
void Scene::DrawScene(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective)
{
    for (unsigned i = 0; i < renderables.size(); ++i)
    {
        renderables[i]->Render(eye, view, perspective);
    }
}

void Scene::MouseMoved(float x, float y)
{
    for (unsigned i = 0; i < renderables.size(); ++i)
    {
        renderables[i]->MouseMoved(x, y);
    }
}

void Scene::MouseClicked(float x, float y)
{
    for (unsigned i = 0; i < renderables.size(); ++i)
    {
        renderables[i]->MouseClicked(x, y);
    }
}

void Scene::ClearFocus()
{
    for (unsigned i = 0; i < renderables.size(); ++i)
    {
        renderables[i]->ClearFocus();
    }
}

void Scene::AddRenderable(std::shared_ptr<Renderable> toAdd)
{
    renderables.push_back(std::move(toAdd));
}

void Scene::RemoveRenderable(std::shared_ptr<Renderable> toRemove)
{
    for (auto i = renderables.begin(); i != renderables.end(); )
    {
        if (*i == toRemove)
        {
            i = renderables.erase(i);
        }
        else
        {
            ++i;
        }
    }
}
