//  This header defines our core application which handles creating our window and updating
//  our scenes as well as routing input to the correct locations. This is essentially the 
//  core state handler of the project.

#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <vector>
#include <sys/time.h>

class GLFWwindow;
class Window;
class Scene;

class Application
{
public:
    ~Application() { }

    //  Initialize runs key startup code for our application
    bool Initialize();

    //  Update handles ticking our clock forward and updating out scenes
    void Update();

    //  Handles Keyobard input by passing it to our windows to deal with as needed
    void HandleKeyInput(int key, int action);
    void MouseDown(int x, int y);
    void MouseMove(int x, int y);
    void MouseUp(bool wasFocused);
    void MouseWheel(int delta);
    void ClearFocus();

    //  Shutdown is our cleanup phase where we return everything we need to the browser
    void Shutdown();

    //  Push a new window onto our Window stack
    void PushWindow(std::shared_ptr<Window> win);

    //  Pop a window off our window stack, returns the window removed
    std::shared_ptr<Window> PopWindow();

    //  Returns our GLFW window for use in rendering
    GLFWwindow* GetGLFWWindow() const { return glfwWindow; }

    //  Get window Width and Height
    inline const int GetWidth() const { return windowWidth; }
    inline const int GetHeight() const { return windowHeight; }

    //  This is one application, we own it, we give out raw pointers
    static Application* Get()
    {
        static std::unique_ptr<Application> singleton;
        if (!singleton)
        {
            singleton.reset(new Application());//std::make_unique<Application>();
        }
        return singleton.get();
    }

private:
    GLFWwindow* glfwWindow;
    std::vector<std::shared_ptr<Window> > windows;
    struct timeval lastTime;
    float timeSinceLastFrameDump;
    unsigned int frameCounter;
    int windowWidth;
    int windowHeight;

    //  Private contructors because this is a singleton
    Application() : glfwWindow(nullptr) { }
};

#endif