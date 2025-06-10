//  This code is the Glue that allows the Javscript side of things to actively call functions
//  on our WASM backend and create and maintain memory and objects to them. This is a bit obtuse
//  and requires some knowledge of the Emscripten Binding API, but generally speaking, its mostly
//  just markup to tell the application what things can be created, what is static, whatg is instance,
//  and the actual function signatures are derived by the compiler.

#include "Application.h"
#include "Scene.h"

#include "Window.h"
#include "JSWindow3D.h"

#include "Renderable.h"
#include "IcoSphereRenderable.h"
#include "CubeRenderable.h"
#include "HexMapRenderable.h"
#include "math_3d.h"
#include "AStarSolver.h"


#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(Application) {
  class_<Application>("Application")
    .class_function("Get", &Application::Get, allow_raw_pointers())
    .function("PushWindow", &Application::PushWindow)
    .function("PopWindow", &Application::PopWindow)
    .function("MouseDown", &Application::MouseDown)
    .function("MouseMove", &Application::MouseMove)
    .function("MouseUp", &Application::MouseUp)
    .function("MouseWheel", &Application::MouseWheel)
    .function("ClearFocus", &Application::ClearFocus)
    ;
}

EMSCRIPTEN_BINDINGS(math_3d){
  value_array<vec3_t>("vec3_t")
    .element(&vec3_t::x)
    .element(&vec3_t::y)
    .element(&vec3_t::z)
    ;
}

EMSCRIPTEN_BINDINGS(Scene) {
  class_<Scene>("Scene")
    .function("AddRenderable", &Scene::AddRenderable)
    .function("RemoveRenderable", &Scene::RemoveRenderable)
    .function("GetRenderableCount", &Scene::GetRenderableCount)
    ;
}

EMSCRIPTEN_BINDINGS(Windows) {
  class_<Window>("Window")
    .smart_ptr<std::shared_ptr<Window>>("Window")
    ;
  class_<JSWindow3D, base<Window>>("JSWindow3D")
    .smart_ptr_constructor("JSWindow3D", &std::make_shared<JSWindow3D>)
    .function("GetScene", &JSWindow3D::GetScene, allow_raw_pointers())
    .function("SetCameraRotate", &JSWindow3D::SetCameraRotate)
    .function("SetCameraPosition", &JSWindow3D::SetCameraPosition)
    ;
}

EMSCRIPTEN_BINDINGS(Renderables) {
  class_<Renderable>("Renderable")
    .smart_ptr<std::shared_ptr<Renderable>>("Renderable")
    .function("SetPosition", &Renderable::SetPosition)
    .function("SetRotation", &Renderable::SetRotation)
    .function("SetScale", &Renderable::SetScale)
    ;
  class_<IcoSphereRenderable, base<Renderable>>("IcoSphereRenderable")
    .smart_ptr_constructor("IcoSphereRenderable", &std::make_shared<IcoSphereRenderable>)
    ;
  class_<CubeRenderable, base<Renderable>>("CubeRenderable")
    .smart_ptr_constructor("CubeRenderable", &std::make_shared<CubeRenderable>)
    ;
  class_<HexMapRenderable, base<Renderable>>("HexMapRenderable")
    .smart_ptr_constructor("HexMapRenderable", &std::make_shared<HexMapRenderable>)
    .function("SetHexTile", &HexMapRenderable::SetHexResourceID)
    .function("SetActiveHexResourceID", &HexMapRenderable::SetActiveHexResourceID)
    .function("ClearMap", &HexMapRenderable::ClearMap)
    .function("GetWorldPositionForHex", &HexMapRenderable::GetWorldPositionForHex)
    .class_function("CalculateHexDistance", &HexMapRenderable::CalculateHexDistance)
    ;
}

EMSCRIPTEN_BINDINGS(AStar) {
  class_<AStarSolver>("AStarSolver")
    .smart_ptr_constructor("AStarSolver", &std::make_shared<AStarSolver>)
    .function("SolvePath", &AStarSolver::SolvePath)
    .function("RenderDebug", &AStarSolver::RenderDebug)
    .function("ClearDebug", &AStarSolver::ClearDebug)
    ;
}