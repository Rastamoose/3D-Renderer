#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/BackgroundShapes.h"
#include "engine/ResourceManager.h"
#include "cube/Cube.h"
#include "Utils.h"
#include "cube/Leaderboard.h"

Leaderboard::Leaderboard(TTFHandler* pttf, const Point& screenSize):       
    changeView(""), 
    backgroundShapes(BackgroundShapes(screenSize, 3, 15, 3, Colour(247,178,165,255), 20, 2)), 
    ttf(pttf),
    cube(Cube(Point(400,200), 400, Colour(136,192,208,255), 3))
    {
        ttf->loadFont("little orion", "./res/fonts/Little Orion.ttf", 128);
        ttf->loadTextWrapped("hello", "little orion", "LEADERBOARD", Colour(235, 159, 148,255));
    }

std::string Leaderboard::update(DisplayManager* display, double dt){
    changeView = "";
    display->setBg(Colour(242, 139, 91, 255));
    backgroundShapes.update(display, dt);
    cube.update(display, dt);
    //display->render(ttf->getText("hello"), Point(300,100));

    if (display->inputs->isPressed(SDL_SCANCODE_ESCAPE)){
        changeView = "mainmenu";
    }

    return changeView;
}
