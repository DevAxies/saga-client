//  This is the implementation of our Application class, it implements our core
//  application loop and calls into our application from this point down

#include "Application.h"
#include <stdio.h>
#include <stdlib.h>
#define  GLFW_INCLUDE_ES3 
#include <GLFW/glfw3.h>
#include "Window.h"
#include <emscripten.h>

//  This function just handles outputting errors from internally for GLFW
static void ErrorCallback(int error, const char* description) 
{
    printf("Error: %s\n", description);
}

//  This function handles keypresses and passing them back into our application
static void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    Application::Get()->HandleKeyInput(key, action);
}

//  Initialize our context and start our application
bool Application::Initialize()
{
    // Setup the Error handler
    glfwSetErrorCallback(ErrorCallback);

    // Start GLFW
    if (!glfwInit()) 
    {
        printf("GLFW Initialization failed.\n");
        return false;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //  Get the window size and resize if it doesn't match
    windowWidth = EM_ASM_INT( 
            return window.innerWidth; 
        );
    windowHeight = EM_ASM_INT( 
            return window.innerHeight; 
        );

    // Create the display window
    glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Mapper", NULL, NULL);
    if (!glfwWindow) 
    {
        printf("GLFW Window Creation Failed.\n");
        glfwTerminate();
        return false;
    }

    // Print GL version
    printf("GL Version: %s\n", glGetString(GL_VERSION));

    // Select the window as the drawing destination
    glfwMakeContextCurrent(glfwWindow);

    // Near white background
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    //  Get initial time
    struct timezone tz;
    gettimeofday ( &lastTime , &tz );
    timeSinceLastFrameDump = 0.0f;
    frameCounter = 0;

    return true;
}

void Application::Update()
{
    //  Don't do this if we have an invalid window
    if (!glfwWindow) return;

    //  Get the window size and resize if it doesn't match
    int width = EM_ASM_INT({ 
            return window.innerWidth; 
        });
    int height = EM_ASM_INT({ 
            return window.innerHeight; 
        });
    if (width != windowWidth || height != windowHeight)
    {
        glfwSetWindowSize(glfwWindow, width, height);
        windowWidth = width;
        windowHeight = height;
    }

    //  Enable Depth buffer
    glEnable(GL_DEPTH_TEST);

    //  Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //  Handle time here
    struct timeval newTime;
    struct timezone tz;
    gettimeofday(&newTime, &tz);
    float dt = (float)(newTime.tv_sec - lastTime.tv_sec + (newTime.tv_usec - lastTime.tv_usec) * 1e-6);
    lastTime = newTime;
    
    //  FPS Counter
    frameCounter++;
    timeSinceLastFrameDump += dt;
    if (timeSinceLastFrameDump > 5.0f)
    {
        //printf("Frames per Second: %d\n", (unsigned int)((float)frameCounter / timeSinceLastFrameDump));
        EM_ASM_({ 
            var fpsElement = document.getElementById('fpscounter');
            if (fpsElement) fpsElement.innerHTML = "FPS: " + $0;
        }, (unsigned int)((float)frameCounter / timeSinceLastFrameDump));
        timeSinceLastFrameDump = 0.0f;
        frameCounter = 0;
    }

    //  Get window information and set our viewport
    //glfwGetFramebufferSize(glfwWindow, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    // If we have a valid window... update it
    if (windows.size() > 0)
    {
        //  Update our top-most window
        windows[windows.size() - 1]->Update(dt);
    }

    //  Present our buffer
    glfwSwapBuffers(glfwWindow);
    
    //  Process any events we need to handle
    glfwPollEvents();
}

void Application::HandleKeyInput(int key, int action)
{
    // If we have a valid window... 
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->HandleKeyPress(key, action);
    }
}

void Application::MouseDown(int x, int y)
{
    // If we have a valid window... 
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->MouseDown(x, y);
    }
}

void Application::MouseMove(int x, int y)
{
    // If we have a valid window...
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->MouseMove(x, y);
    }
}

void Application::MouseUp(bool wasFocused)
{
    // If we have a valid window...
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->MouseUp(wasFocused);
    }
}

void Application::MouseWheel(int delta)
{
    // If we have a valid window...
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->MouseWheel(delta);
    }
}

void Application::ClearFocus()
{
    // If we have a valid window...
    if (windows.size() > 0)
    {
        //  Push input into the top most window
        windows[windows.size() - 1]->ClearFocus();
    }
}

void Application::Shutdown()
{
    if (glfwWindow)
    {
        glfwDestroyWindow(glfwWindow);
    }
    glfwTerminate();
}

void Application::PushWindow(std::shared_ptr<Window> win)
{
    windows.push_back(std::move(win));
}

std::shared_ptr<Window> Application::PopWindow()
{
    // If we have a valid window...
    if (windows.size() > 0)
    {
        std::shared_ptr<Window> out = std::move(windows[windows.size() - 1]);
        windows.pop_back();    
        return std::move(out);
    }

    return nullptr;
}