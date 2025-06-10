//  Main entry point for our application, handles the basic glue for runniong a 
//  Enscripten compiled and built application, we call into our own Applicationm
//  class for abstracting the majority of this from our own application.

//  Include some core libraries
#include <stdio.h>
#include <stdlib.h>

//  This provides us with functions specifically associated with Emscripten
#include <emscripten/emscripten.h>

//  This is our primary application class, it creates and manages our window, it is
//  a singleton as we only have one application.
#include "Application.h"

static void TickFrame()
{
    Application::Get()->Update();
}

int main()
{
    printf("Starting Application\n");

    //  Create and get a copy of our application
    Application* application = Application::Get();

    //  Initialize our Application
    if (!application->Initialize())
    {
        printf("Failed to initialize application, exiting now.\n");
        emscripten_force_exit(EXIT_FAILURE);
    }

    //  Run our primary loop for Emscripten
    emscripten_set_main_loop(TickFrame, 0, false);
}