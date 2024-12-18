#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "3D/Utils3D.h"
#include "engine/OBJHandler.h"

Obj::Obj(): translationMat(Mat4x4()), scalingMat(Mat4x4()){}

void Obj::translate(float x, float y, float z){
    translationMat.m[0][3] = x;
    translationMat.m[1][3] = y;
    translationMat.m[2][3] = z;
}

void Obj::scale(float x, float y, float z){
    scalingMat.m[0][0] = x;
    scalingMat.m[1][1] = y;
    scalingMat.m[2][2] = z;
}

void Obj::updateOutVecs(){
    for(auto& vec:vecs){
        outVecs.push_back({vec.x,vec.y,vec.z,1});
    }
}

OBJHandler::OBJHandler(){
    std::cout << "<OBJHANDLER> OBJ initialised.\n";
    
}

OBJHandler::~OBJHandler(){
    if(models.size()>0){
        for(auto& model:models){
            delete model.second;
        }
    }
    std::cout << "<OBJHANDLER> Models successfully destroyed.\n";
}

void OBJHandler::triangulate(std::vector<int>& face, const std::string& name){
    for(int i=1;i<static_cast<int>(face.size());i++){
        std::vector<int> tri = {face.at(0), face.at(i), face.at(i+1)};
        getModel(name)->faces.push_back(tri);
    }
}

int OBJHandler::loadModel(const std::string& name, const char* fName){
    std::ifstream file(fName);
    if(!file){
        std::cout << "<OBJHANDLER> Failed to load a model. Error: Error opening file\n"; 
        return 1;
    }

    models.emplace(name, new Obj());

    // go through each line
    std::string line;
    while(std::getline(file,line)){
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        
        // load vertex
        if(token=="v"){
            // convert each string to float
            float x,y,z;
            iss >> token;   x = std::stof(token);
            iss >> token;   y = std::stof(token);
            iss >> token;   z = std::stof(token);
            getModel(name)->vecs.push_back({x,y,z});      
        }
        else if(token=="f"){
            std::vector<int> face;
            while(iss >> token){
                std::istringstream ss(token);
                std::string index;
                std::getline(ss, index, '/');
                int vIndex = std::stoi(index)-1;        // -1 as .obj are 1 indexed
                face.push_back(vIndex);
            }
            // check if face needs to be triangulated
            if(face.size()>3){
                triangulate(face, name);
            }else{
                getModel(name)->faces.push_back(face);
            }
        }

    }

    getModel(name)->updateOutVecs();
    return 0;
}

void OBJHandler::delModel(const std::string& name){
    delete models.at(name);
    models.erase(name);
}

inline Obj* OBJHandler::getModel(const std::string& name){
    return models.at(name);
}                    

void OBJHandler::getAllModels(std::vector<Obj*>& objs){
    if(models.size()==0){return;}
    // reserve capacity to avoid unecessary resizing
    objs.reserve(models.size());
    
    for(auto& model:models){
        objs.push_back(model.second);
    }
}


