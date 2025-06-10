**Introduction:**

This is an Emscripten based Game / Rendering Engine, written in C++ with bindings exposed out to JS to 
create, manage, and work with Scenes and Renderables. This specific example shows the JS creating a window,
adding a Hex Map to it, as well as a Player Sphere, and then exposes the following controls to allow 
manipulation of the map, and the ability to run an A Star Solver from the Player to a Target Hex.

- Left Click: Move the player to the selected tile
- Right Click: Move the Target Hex to the selected tile
- Left Click + Drag: Move the map around
- Scroll Wheel: Zoom in and out
- A Key: Set the tile you are hovering over to "Clear"
- S Key: Set the tile you are hovering over to "Impassable"
- D Key: Set the tile you are hovering over to "Difficult" 
- Space Key: Runs the A Star Path Solver

Additionally there are two buttons provided in the top right that will clear the debug rendering of the 
A Star Pathfinding, and one that will reset the map back to the default value of every tile being "clear"

**Example Video:** https://youtu.be/-_csegEkleM

**Files of Note:**
- out/client.html - This file is hand edited and contains the Javascript that drives this demo by creating our Window, Adding Renderables, and Calling the A Star Path Solver.
- JSWindow3D.h/.coo - This is the primary C++ Window created by the Javascript front end and handles input passing from Javascript to the Application
- HexMapRenderable.h/.cpp - This is the primary Hex Map used by the application and includes all the logic behind the Hex Grid and how it is Rendered
- AStarSolver.h/.cpp - This is the exposed class that will solve A Star on our Hex Grid, it is created and told to solve by Javascript
- Binding.cpp - This contains all of the core bindings from C++ to Javascript allowing for the communication between these layers


**Acknowledgements:**

math_3d.h/.cpp by Stephan Soller under the MIT License and modified by David Hancock as needed for Emscripten. 


**Instructions:**
 1. Install Python (3.6+), git, and Emscripten (https://emscripten.org/docs/getting_started/downloads.html)
 2. Open a Command Prompt Window and navigate to the project root folder
 3. If it doesn't exist, create a new folder called "out"
 4. Run the following command line: 
       em++ --bind src/math_3d.cpp src/Shader.cpp src/Texture2D.cpp src/Vertex.cpp src/Mesh.cpp src/IcoSphere.cpp src/BaseProgram.cpp src/BasicShader.cpp src/UnlitShader.cpp src/Camera.cpp src/Renderable.cpp src/CubeRenderable.cpp src/IcoSphereRenderable.cpp src/HexMapRenderable.cpp src/AStarSolver.cpp src/Scene.cpp src/Application.cpp src/Window.cpp src/JSWindow3D.cpp src/main.cpp src/Binding.cpp -o out\client.js -std=c++11 -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file assets --use-preload-plugins

 5. In the "out" folder there should be 4 files, you will need a basic webserver to run these
 5a. If you run the following command from commandline in the out directory it should create a local web server and open the page:
        "emrun client.html"


**Examples of Usage:**

In Javascript we need to start by creating our Window, this should be done inside the Emscripten Module objects PostRun callback:
```Javacript
var jsRenderWindow = new Module.JSWindow3D();
Module.Application.Get().PushWindow(jsRenderWindow);
```
You can optionally set the Camera for this Window to a specific location like so:
```Javascript
jsRenderWindow.SetCameraPosition(-0.4, -0.4, -15.0);
```
Now with a valid JSWindow3D that has been added to our applications Window stack, and with the camera set, we can add renderables to our Window, such as the following code:
```Javascript
var mapRenderable = null;
function CreateHexMap(){
    //  If there is an existing map, clean it up
    if (mapRenderable)
    {
        jsRenderWindow.GetScene().RemoveRenderable(mapRenderable);
        mapRenderable.delete();
    }

    //  Allocate a new map renderable, clear it to Tile Resource 2
    mapRenderable = new Module.HexMapRenderable();
    mapRenderable.ClearMap(2);

    //  Add the map renderable to the scene
    jsRenderWindow.GetScene().AddRenderable(mapRenderable);
}

CreateHexMap()
```
You can set a specific tile index using this function from Javascript:
```Javascript
// Set the Hex at {10, 15} to Hex Resource ID 1
mapRenderable.SetHexTile(10, 15, 1);
```

Now you may be wondering what these methods might look like from C++, here is what AddRenderable looks like:
```cpp
void Scene::AddRenderable(std::shared_ptr<Renderable> toAdd)
{
    renderables.push_back(std::move(toAdd));
}
```
As you can see from this, we are adding a std::shared_ptr that is getting passed and created by Javascript, this renderable is added to the Scene's internal list of Renderables and then Updated and rendered as part odf the Scenes regular update and render methods, all inside WASM, written in C++. 

For Input Pass through, here is an example from Javascript:
```Javascript
var isMouseOverCanvas = true;
window.addEventListener('mousemove', function(event){
    //  In the case of mouse moves, we add a failure case for when the mouse moves over UI from the Canvas,
    //  this is to let the Canvas know that the mouse has lost focus on it and to clear and selection states.
    var elementMouseIsOver = document.elementFromPoint(event.clientX, event.clientY);
    if (elementMouseIsOver != null && elementMouseIsOver.id == "canvas")
    {
        isMouseOverCanvas = true;
        Module.Application.Get().MouseMove(event.clientX, event.clientY);
    }
    else if (isMouseOverCanvas)
    {
        Module.Application.Get().ClearFocus();
        isMouseOverCanvas = false;
    }
      
    event.stopImmediatePropagation();
}, true);
```
This is how the HexMapRenderable handles that above passed through movement from Javascript:
```cpp
void HexMapRenderable::MouseMoved(float x, float y) 
{ 
    int hexX = -1, hexY = -1;
    GetClosestHexToPoint(x, y, &hexX, &hexY);

    if (hexX != highlightX || hexY != highlightY)
    {
        highlightX = hexX;
        highlightY = hexY;
    }
}
```
