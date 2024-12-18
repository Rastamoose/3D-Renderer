#include <iostream>
#include <cmath>
#include "3D/Utils3D.h"

Vec3::Vec3(float px, float py, float pz): x(px), y(py), z(pz){} 

Vec3 Vec3::operator=(const Vec3& b){
    // self assignment
    if(this==&b){return *this;}

    x=b.x;  y=b.y;  z=b.z;

    return *this;
}

Vec3 Vec3::operator+(const Vec3& b) const{
    return Vec3(x+b.x,y+b.y,z+b.z);
}

Vec3 Vec3::operator-(const Vec3& b) const{
    return Vec3(x-b.x,y-b.y,z-b.z);
}

Vec3 Vec3::operator-() const{
    return Vec3(-x,-y,-z);
}

Vec3 Vec3::operator*(float scalar) const{
    return Vec3(x*scalar,y*scalar,z*scalar);
}

float Vec3::dot(const Vec3& b){
    return x*b.x + y*b.y + z*b.z;
}

float Vec3::mag(){
    return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::cross(const Vec3& b){
    return Vec3(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
}

Vec3 Vec3::normalise(){
    float mag_ = mag();
    return Vec3(x/mag_, y/mag_, z/mag_);
    return *this;
}

void Vec3::print(){
    std::cout << "{" << x << "," << y << "," << z << "}\n";
}

Vec4::Vec4(float px, float py, float pz, float pw): x(px), y(py), z(pz), w(pw){} 

Vec4 Vec4::operator=(const Vec4& b){
    // self assignment
    if(this==&b){return *this;}

    x=b.x;  y=b.y;  z=b.z;  w=b.w;

    return *this;
}

Vec4 Vec4::operator+(const Vec4& b) const{
    return Vec4(x+b.x,y+b.y,z+b.z,w+b.w);
}

Vec4 Vec4::operator-(const Vec4& b) const{
    return Vec4(x-b.x,y-b.y,z-b.z,w-b.w);
}

Vec4 Vec4::operator*(float scalar) const{
    return Vec4(x*scalar,y*scalar,z*scalar,w*scalar);
}

float Vec4::dot(const Vec4& b){
    return x*b.x + y*b.y + z*b.z + w*b.w;
}

float Vec4::mag(){
    return sqrt(x*x + y*y + z*z + w*w);
}


Vec4 Vec4::normalise(){
    float mag_ = mag();
    return Vec4(x/mag_, y/mag_, z/mag_, w/mag_);
    return *this;
}

void Vec4::print(){
    std::cout << "{" << x << "," << y << "," << z << "," << w << "}\n";
}



// construct identity mat
Mat4x4::Mat4x4(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(i==j){
                m[i][j] = 1.0f;
            }else{
                m[i][j] = 0.0f;
            }
        }
    }
}

Vec4 Mat4x4::operator*(const Vec4& v) const{
    return Vec4(m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
                m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
                m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
                m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w);
}

Mat4x4 Mat4x4::operator*(const Mat4x4& b) const{
    Mat4x4 out;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            out.m[i][j] = 0;
            for(int k=0;k<4;k++){
                out.m[i][j] += m[i][k] * b.m[k][j];
            }
        }
    }
    return out;
}
