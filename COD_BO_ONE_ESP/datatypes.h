#pragma once
#include <cstddef>
#include <cstdint>
#include "Windows.h"

struct Vec2 {
    float X;
    float Y;

    Vec2() : X(0), Y(0) {} // Default constructor
    Vec2(float x, float y) : X(x), Y(y) {} // Constructor with parameters
};


struct Vec3 {
    float X;
    float Y;
    float Z;

    Vec3() : X(0), Y(0), Z(0) {} // Default constructor
    Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    // Overload the - operator
    Vec3 operator-(const Vec3& other) const {
        return Vec3(X - other.X, Y - other.Y, Z - other.Z);
    }
};

struct Vec4 {
    float X;
    float Y;
    float Z;
    float W;
};


struct ViewMatrix {
    float Matrix[16]{};
};

struct Entity {
    int health;
    int maxHealth;
    uintptr_t baseAdress;
    Vec3 Origin;
    Vec2 HeadCoords;
    Vec2 FeetCoords;
    Vec2 NeckCoords;
    Vec3 headOrigin;
    bool bAlive;
};

struct Player {
    Vec2 Angles{};
    Vec3 Origin{};
    ViewMatrix Matrix{};
    uintptr_t baseAdress;
};