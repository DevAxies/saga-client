Introduction: 
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

Note that the client.html file inside the "out" folder is actually a static file and contains the working JS code
that makes this demo work.


Acknowledgements: 
math_3d.h/.cpp by Stephan Soller under the MIT License and modified by David Hancock as needed for Emscripten. 


Instructions:
 1. Install Python (3.6+), git, and Emscripten (https://emscripten.org/docs/getting_started/downloads.html)
 2. Open a Command Prompt Window and navigate to the project root folder
 3. If it doesn't exist, create a new folder called "out"
 4. Run the following command line: 
       em++ --bind src/math_3d.cpp src/Shader.cpp src/Texture2D.cpp src/Vertex.cpp src/Mesh.cpp src/IcoSphere.cpp src/BaseProgram.cpp src/BasicShader.cpp src/UnlitShader.cpp src/Camera.cpp src/Renderable.cpp src/CubeRenderable.cpp src/IcoSphereRenderable.cpp src/HexMapRenderable.cpp src/AStarSolver.cpp src/Scene.cpp src/Application.cpp src/Window.cpp src/JSWindow3D.cpp src/main.cpp src/Binding.cpp -o out\client.js -std=c++11 -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file assets --use-preload-plugins

 5. In the "out" folder there should be 4 files, you will need a basic webserver to run these
 5a. If you run the following command from commandline in the out directory it should create a local web server and open the page:
        "emrun client.html"