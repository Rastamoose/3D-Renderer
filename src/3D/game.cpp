#include <string>
#include <SDL2/SDL.h>
#include "engine/View.h"
#include "engine/WindowManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "3D/Engine3D.h"
#include "3D/Game.h"

Game::Game(const Point& screenSize, WindowManager* window, ResourceManager* presources):       
    changeView(""), resources(presources), 
    backgroundShapes(BackgroundShapes(screenSize, 3, 15, 3, Colour(67,76,94,255), 20, 2)), 
    engine(Engine3D(window,resources->obj)){}

std::string Game::update(DisplayManager* display, double dt){
    changeView = "";
    display->setBg(Colour(46, 52, 73, 255));
    //backgroundShapes.update(display, dt);
    engine.update(display, dt);

    if (display->inputs->isPressed(SDL_SCANCODE_ESCAPE)){
        changeView = "mainmenu";
    }

    return changeView;
}