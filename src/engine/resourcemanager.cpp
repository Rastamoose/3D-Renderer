#include <iostream>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine/IMGHandler.h"
#include "engine/TTFHandler.h"
#include "engine/ResourceManager.h"


ResourceManager::ResourceManager(SDL_Renderer* prenderer):
    renderer(prenderer), img(nullptr), ttf(nullptr), obj(nullptr){}

ResourceManager::~ResourceManager(){
    delete obj;
    delete ttf;
    delete img;
}

void ResourceManager::initIMG(){
    if(img == nullptr){
        img = new IMGHandler(renderer);
    } else{
        std::cout << "<RESOURCEMANAGER> IMGHandler already initialised.\n";
    }
}

void ResourceManager::initTTF(){
    if(ttf == nullptr){
        ttf = new TTFHandler(renderer);
    } else{
        std::cout << "<RESOURCEMANAGER> TTFHandler already initialised.\n";
    }
}

void ResourceManager::initOBJ(){
    if(obj == nullptr){
        obj = new OBJHandler();
    } else{
        std::cout << "<RESOURCEMANAGER> OBJHandler already initialised.\n";
    }
}

