#include <SDL2/SDL_scancode.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Utils.h"
#include "engine/DisplayManager.h"
#include "3D/Engine3D.h"

Camera::Camera(){};

Engine3D::Engine3D(WindowManager* pwindow, OBJHandler* pobjHandler): window(pwindow), objHandler(pobjHandler), projMat(Mat4x4()){
    initProjMat();
    initViewMat();
    objHandler->loadModel("cube", "res/obj/Cube.obj");
}

void Engine3D::initProjMat(){
    Point screenSize = window->getSize();
    float ar = (screenSize.x*1.0f) / screenSize.y;      // aspect ratio
    float fovRad = (FOV*PI)/180.0f;
    float f = 1/(tan(fovRad/2));
    float zRange = ZFAR-ZNEAR;

    projMat = Mat4x4();      // reset to identity matrix

    // initialise projection matrix (everything else is 0)
    projMat.m[0][0] = (1/ar)*f;
    projMat.m[1][1] = f;
    projMat.m[2][2] = -(ZFAR+ZNEAR)/zRange;
    projMat.m[2][3] = -(2*ZFAR*ZNEAR)/zRange;
    projMat.m[3][2] = -1.0f;
    projMat.m[3][3] = 0.0f;
}

void Engine3D::initViewMat(){
    // reset to identity matrix
    viewMat = Mat4x4();
    // forward (z) axis - normalise target-pos - z direction
    Vec3 zAxis = (camera.pos - camera.target).normalise();
    // right (x) axis - normalise cross of up and forward - x direction
    Vec3 xAxis = (camera.up.cross(zAxis)).normalise();
    // true up (y) axis - cross of forward and right (dont need to normalise as others already are)
    Vec3 yAxis = zAxis.cross(xAxis); 

    // rotation part (column-major)
    viewMat.m[0][0] = xAxis.x; viewMat.m[0][1] = yAxis.x; viewMat.m[0][2] = zAxis.x; viewMat.m[0][3] = 0.0f;
    viewMat.m[1][0] = xAxis.y; viewMat.m[1][1] = yAxis.y; viewMat.m[1][2] = zAxis.y; viewMat.m[1][3] = 0.0f;
    viewMat.m[2][0] = xAxis.z; viewMat.m[2][1] = yAxis.z; viewMat.m[2][2] = zAxis.z; viewMat.m[2][3] = 0.0f;

    // translation part (last column)
    viewMat.m[3][0] = -xAxis.dot(camera.pos);
    viewMat.m[3][1] = -yAxis.dot(camera.pos);
    viewMat.m[3][2] = -zAxis.dot(camera.pos);
    viewMat.m[3][3] = 1.0f;
}

float Engine3D::edgeFunction(const Point& a, const Point& b, const Point& c){
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

void Engine3D::render(DisplayManager* display, Obj* obj){
    // combine object translation and scaling matrices
    Mat4x4 modelMat = obj->scalingMat * obj->translationMat;

    // process vertices
    for(int i=0; i<static_cast<int>(obj->vecs.size());i++){
        obj->outVecs[i] = Vec4(obj->vecs[i].x, obj->vecs[i].y, obj->vecs[i].z, 1);
        // apply matrices in order
        obj->outVecs[i] = modelMat*obj->outVecs[i];
        obj->outVecs[i] = viewMat*obj->outVecs[i];   
        obj->outVecs[i] = projMat*obj->outVecs[i];
        perspectiveDiv(obj->outVecs[i]);
        viewportTransform(window->getSize(), obj->outVecs[i]);
    }
    // now that vertices are in suitable form to put on screen, rasterise
    rasterise(display, obj);
}

void Engine3D::perspectiveDiv(Vec4& vec){
    if(vec.w != 0.0f){
        vec.x /= vec.w; 
        vec.y /= vec.w; 
        vec.z /= vec.w;
    }
}


void Engine3D::viewportTransform(const Point& sSize, Vec4& vec){
    float halfW = sSize.x/2.0f;
    float halfH = sSize.y/2.0f;
    vec.x = (vec.x+1.0f)*halfW;
    vec.y = (1.0f-vec.y)*halfH;
}


void Engine3D::rasterise(DisplayManager* display, const Obj* obj){
    for(auto& v:obj->outVecs){
        display->drawThickPoint(Point(v.x,v.y), thickness, colour);
    }
    for(auto& tri:obj->faces){
        Point a = Point(obj->outVecs.at(tri.at(0)).x,obj->outVecs.at(tri.at(0)).y);
        Point b = Point(obj->outVecs.at(tri.at(1)).x,obj->outVecs.at(tri.at(1)).y);
        Point c = Point(obj->outVecs.at(tri.at(2)).x,obj->outVecs.at(tri.at(2)).y);
        //fillTriangle(display,a,b,c);
        //std::cout << "{" << a.x << "," << a.y << "}" << std::endl;
        //std::cout << "{" << b.x << "," << b.y << "}" << std::endl;
        //std::cout << "{" << c.x << "," << c.y << "}" << std::endl;
        //std::cout << std::endl;
        display->drawThickLine(Point(a.x,a.y), Point(b.x,b.y), thickness, colour);
        display->drawThickLine(Point(b.x,b.y), Point(c.x,c.y), thickness, colour);
        display->drawThickLine(Point(c.x,c.y), Point(a.x,a.y), thickness, colour);
    }
}

void Engine3D::fillTriangle(DisplayManager* display, const Point& a, const Point& b, const Point& c){
    Point vMin = {0,0};
    Point vMax = {0,0};
    // calculate minimum bounding rect of tri a,b,c
    vMin.x = std::min(a.x, std::min(b.x, c.x));
    vMin.y = std::min(a.y, std::min(b.y, c.y));
    vMax.x = std::max(a.x, std::max(b.x, c.x));
    vMax.y = std::max(a.y, std::max(b.y, c.y));

    // go through each pixel within minimum bounding rect
    for(int y=vMin.y;y<vMax.y;y++){
        for(int x=vMin.x;x<vMax.x;x++){
            // only draw if pixel is within ab, bc and ac
            Point p = {x,y};
            float abp = edgeFunction(a,b,p);
            float bcp = edgeFunction(b,c,p);
            float cap = edgeFunction(c,a,p);
            if(abp>=0 && bcp>=0 && cap>=0){
                display->drawThickPoint(p, thickness, colour);
            }
        }
    }    
}

void Engine3D::update(DisplayManager* display, double dt){
    std::vector<Obj*> objs;
    objHandler->getAllModels(objs);
    initProjMat();
    initViewMat();

    objs.at(0)->translate(5,0,0);

    // render every object
    for(Obj* obj:objs){
        render(display, obj);
    }

    int move = 10;
    if(display->inputs->isPressed(SDL_SCANCODE_R)){
        camera.moveTo(0,0,0);
    }

    if(display->inputs->isPressed(SDL_SCANCODE_RIGHT)){
        camera.move(move,0,0);
    }
    if(display->inputs->isPressed(SDL_SCANCODE_LEFT)){
        camera.move(-move,0,0);
    }
    if(display->inputs->isPressed(SDL_SCANCODE_UP)){
        camera.move(0,0,move);
    }
    if(display->inputs->isPressed(SDL_SCANCODE_DOWN)){
        camera.move(0,0,-move);
    }
    if(display->inputs->isPressed(SDL_SCANCODE_SPACE)){
        camera.move(0,move,0);
    }
    if(display->inputs->isPressed(SDL_SCANCODE_LSHIFT)){
        camera.move(0,0,-move);
    }
}