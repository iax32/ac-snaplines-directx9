#pragma once
#include "datatypes.h"

bool WorldToNDC(const Vec3 VecOrigin, Vec2& VecNDC, float* Matrix);
bool WorldToScreenEsp(const Vec3 VecOrigin, Vec2& VecScreen, float* Matrix, int ScreenWidth, int ScreenHeight);
