#ifndef ENGINE3D_H
#define ENGINE3D_H

// DEFINE VIEW CONSTANTS
#define PI 3.14159f
#define ZNEAR 0.1f
#define ZFAR 1000.0f
#define FOV 90

#include <cmath>
#include <vector>
#include "Utils.h"
#include "engine/WindowManager.h"
#include "engine/DisplayManager.h"
#include "engine/OBJHandler.h"
#include "3D/Utils3D.h"
// Conversions cause points to lose accuracy over time and thus become smaller (use -Wconversion flag to check)

struct Camera{
    Vec3 pos = {0,0,-10};
    Vec3 target = {0,0,0};
    Vec3 up = {0,1,0};

    Camera();
    void move(float x, float y, float z){pos.x+=x;pos.y+=y;pos.z+=z;target.x-=x;target.y-=y;target.z-=z;}
    void moveTo(float x, float y, float z){pos.x=x;pos.y=y;pos.z=z;target.x=-x;target.y=-y;target.z=-z;}
};

class Engine3D{
    WindowManager* window;
    OBJHandler* objHandler;
    Colour colour = {0,0,0,255};
    int thickness = 5;
    Obj cube = Obj();
    Camera camera = Camera();
    Mat4x4 projMat;
    Mat4x4 viewMat;     // lookAt matrix

    void initProjMat();
    void initViewMat();
    float edgeFunction(const Point& a, const Point& b, const Point& c);
    // render all objects
    // projection matrix -> perspective divide
    void render(DisplayManager* display, Obj* obj);
    // perspective divide
    void perspectiveDiv(Vec4& vec);
    // scale from NDC coordinate [-1,1] to the actual 2d screen
    void viewportTransform(const Point& sSize, Vec4& vec);
    void rasterise(DisplayManager* display, const Obj* obj);
    // fill pixels within each triangle
    void fillTriangle(DisplayManager* display, const Point& a, const Point& b, const Point& c);

    public:
    Engine3D(WindowManager* pwindow, OBJHandler* pobjHandler);
    void update(DisplayManager* display, double dt);
};

#endif