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
    // normalise target-pos - z direction
    Vec3 forward = (camera.target-camera.pos).normalise();
    // normalise cross of up and forward - x direction
    Vec3 right = (camera.up.cross(forward)).normalise();
    // cross of forward and right (dont need to normalise as others already are) - y direction
    Vec3 up = forward.cross(right);

    // initialise view matrix
    viewMat.m[0][0]=right.x;    viewMat.m[0][1]=right.y;   viewMat.m[0][2]=right.z;
    viewMat.m[1][0]=up.x;    viewMat.m[1][1]=up.y;   viewMat.m[1][2]=up.z;
    viewMat.m[2][0]=forward.x;    viewMat.m[2][1]=forward.y;   viewMat.m[2][2]=forward.z;
    viewMat.m[3][0]=right.dot(camera.pos);    
    viewMat.m[3][1]=up.dot(camera.pos);   
    viewMat.m[3][2]=forward.dot(camera.pos);   
    viewMat.m[3][3]=1.0f;
}

float Engine3D::edgeFunction(const Point& a, const Point& b, const Point& c){
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}


void Engine3D::render(DisplayManager* display, Obj* obj){
    // combine object translation and scaling matrices
    Mat4x4 modelMat = obj->translationMat * obj->scalingMat;

    // process vertices
    for(int i=0; i<static_cast<int>(obj->vecs.size());i++){
        obj->outVecs[i] = Vec4(obj->vecs[i].x, obj->vecs[i].y, obj->vecs[i].z, 1);


        // apply matrices in order
        obj->outVecs[i] = modelMat*obj->outVecs[i];
        obj->outVecs[i] = viewMat*obj->outVecs[i];
        obj->outVecs[i] = projMat*obj->outVecs[i];
        perspectiveDiv(obj->outVecs[i]);
        viewportTransform(window->getSize(), obj->outVecs[i]);
        obj->outVecs[i].print();
    }
    std::cout << "\n\n";
    // now vertices are in suitable form to put on screen, rasterise
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
    //     Point vMin = {0,0};
    //     Point vMax = {0,0};
    //     vMin.x = std::min(a.x, std::min(b.x, c.x));
    //     vMin.y = std::min(a.y, std::min(b.y, c.y));
    //     vMax.x = std::max(a.x, std::min(b.x, c.x));
    //     vMax.y = std::max(a.y, std::min(b.y, c.y));

    //     for(int y=vMin.y;y<vMax.y;y++){
    //         for(int x=vMin.x;x<vMax.x;x++){
    //             Point p = {x,y};
    //             float abp = edgeFunction(a,b,p);
    //             float bcp = edgeFunction(b,c,p);
    //             float cap = edgeFunction(c,a,p);
    //             if(abp>=0 && bcp>=0 && cap>=0){
    //                 display->drawThickPoint(p, thickness, colour);
    //             }
    //         }
    //     }
        display->drawThickLine(Point(a.x,a.y), Point(b.x,b.y), thickness, colour);
        display->drawThickLine(Point(b.x,b.y), Point(c.x,c.y), thickness, colour);
        display->drawThickLine(Point(c.x,c.y), Point(a.x,a.y), thickness, colour);
    }
}


void Engine3D::update(DisplayManager* display, double dt){
    std::vector<Obj*> objs;
    objHandler->getAllModels(objs);
    initProjMat();
    initViewMat();

    objs.at(0)->scale(10.0f,10.0f,10.0f);
    // render every object
    for(Obj* obj:objs){
        render(display, obj);
    }
}