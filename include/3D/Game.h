#ifndef GAME_H
#define GAME_H

#include <string>
#include "engine/View.h"
#include "engine/WindowManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/BackgroundShapes.h"
#include "3D/Engine3D.h"

class Game : public View{
    std::string changeView;
    ResourceManager* resources;
    BackgroundShapes backgroundShapes;
    Engine3D engine;

    void updateButtons(DisplayManager* display, double dt);

    public:
    Game(const Point& screenSize, WindowManager* window, ResourceManager* presources);
    std::string update(DisplayManager* display, double dt);
};

#endif