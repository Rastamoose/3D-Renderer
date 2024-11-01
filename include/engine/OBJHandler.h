#ifndef OBJHANDLER_H
#define OBJHANDLER_H

#include <unordered_map>
#include <vector>
#include <string>
#include "3D/Utils3D.h"

struct Obj{
    std::vector<Vec3> vecs;
    std::vector<Vec4> outVecs;     // vectors outputted after rendering pipeline
    std::vector<Connection> connections;
    std::vector<std::vector<int> > faces;
    Mat4x4 translationMat;
    Mat4x4 scalingMat;

    Obj();
    void updateOutVecs();
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
};

// TODO: ADD SUPPORT FOR INITAL LINE POINTING TO TEXTURE AND SHADER FILES
class OBJHandler{
    std::unordered_map<std::string, Obj*> models;
    // convert a face of n vertices into triangles and push to obj faces
    // implemented using fan triangulation
    void triangulate(std::vector<int>& face, const std::string& name);

    public:
    OBJHandler();           
    ~OBJHandler();                     
    int loadModel(const std::string& name, const char* fName);     // load model and add to model hashmap
    void delModel(const std::string& name);
    inline Obj* getModel(const std::string& name);                        // return corresponding model
    void getAllModels(std::vector<Obj*>& objs);
};

#endif