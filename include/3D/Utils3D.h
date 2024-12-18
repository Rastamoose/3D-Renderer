#ifndef UTILS3D_H
#define UTILS3D_H

struct Connection{
    int a,b;
};

struct Vec3{
    float x,y,z;

    Vec3(float px, float py, float pz);
    Vec3 operator=(const Vec3& b);
    Vec3 operator+(const Vec3& b) const;
    Vec3 operator-(const Vec3& b) const;    // subtraction
    Vec3 operator-() const;                 // negation
    Vec3 operator*(float scalar) const;
    float dot(const Vec3& b);  
    float mag();  
    Vec3 cross(const Vec3& b);
    Vec3 normalise();
    void print();
};

struct Vec4{
    float x,y,z,w;

    Vec4(float px, float py, float pz, float pw);
    Vec4 operator=(const Vec4& b);
    Vec4 operator+(const Vec4& b) const;
    Vec4 operator-(const Vec4& b) const;
    Vec4 operator*(float scalar) const;
    float dot(const Vec4& b);  
    float mag();  
    Vec4 normalise();
    void print();
};

struct Mat4x4{
    float m[4][4];

    // construct identity matrix by default
    Mat4x4();
    Vec4 operator*(const Vec4& v) const;
    Mat4x4 operator*(const Mat4x4& b) const;
};

#endif