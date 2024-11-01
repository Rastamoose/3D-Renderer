#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Utils.h"
#include "engine/WindowManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/AnimationHandler.h"
#include "engine/TimeManager.h"
#include "engine/ViewManager.h"
#include "mainmenu/MainMenu.h"
#include "3D/Game.h"

int main(int argc, char* argv[]) {

    WindowManager window = WindowManager("Project", Rect(200, 200, 1280, 720), SDL_WINDOW_SHOWN);
    ResourceManager resources = ResourceManager(window.getRenderer());
    resources.initIMG();
    resources.initTTF();
    resources.initOBJ();
    DisplayManager display = DisplayManager(&window);    
    TimeManager time = TimeManager(60);

    // initialise views
    ViewManager viewManager = ViewManager();

    MainMenu mainMenu = MainMenu(&resources, display.getSize());
    Game game = Game(display.getSize(), &window, &resources);
    
    viewManager.addView("mainmenu", &mainMenu);
    viewManager.addView("game", &game);
    viewManager.setCurrentView("mainmenu");

    // Game loop
    bool running = true;
    while(running){
        time.updateStart();

        window.inputs.pollEvents();
        if(window.inputs.quit || viewManager.isQuit){
            running = false;
        }

        window.clear();
        viewManager.updateView(&display, time.dt);
        window.display();

        time.updateTime();

    }

    return 0;
}