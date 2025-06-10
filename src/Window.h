//  Whindow class, this is a base class, it is not intended for use, it is pure virtual,
//  overload it and win by being cool, don't and nothing happens, you just not cool

#ifndef WINDOW_H
#define WINDOW_H

class Scene;

class Window 
{
public:
    Window();
    virtual ~Window();

    virtual void Update(float dt) = 0;
    virtual void HandleKeyPress(int key, int action) = 0;
    virtual void MouseDown(int x, int y) = 0;
    virtual void MouseMove(int x, int y) = 0;
    virtual void MouseUp(bool wasFocused) = 0;
    virtual void MouseWheel(int delta) = 0;
    virtual void ClearFocus() = 0;
    virtual Scene* GetScene() const = 0;

private:
    

};

#endif