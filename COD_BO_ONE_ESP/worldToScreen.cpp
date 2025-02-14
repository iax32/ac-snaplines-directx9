#include "worldToScreen.h"

bool WorldToNDC(const Vec3 VecOrigin, Vec2& VecNDC, float* Matrix) {
    Vec4 clipCoords = {};

    // Transform world coordinates to clip space
    clipCoords.X = VecOrigin.X * Matrix[0] + VecOrigin.Y * Matrix[4] + VecOrigin.Z * Matrix[8] + Matrix[12];
    clipCoords.Y = VecOrigin.X * Matrix[1] + VecOrigin.Y * Matrix[5] + VecOrigin.Z * Matrix[9] + Matrix[13];
    clipCoords.Z = VecOrigin.X * Matrix[2] + VecOrigin.Y * Matrix[6] + VecOrigin.Z * Matrix[10] + Matrix[14];
    clipCoords.W = VecOrigin.X * Matrix[3] + VecOrigin.Y * Matrix[7] + VecOrigin.Z * Matrix[11] + Matrix[15];

    // Check if object is behind the camera
    if (clipCoords.W < 0.1f) {
        return false;
    }

    // Perform perspective division to get Normalized Device Coordinates (NDC)
    VecNDC.X = clipCoords.X / clipCoords.W;
    VecNDC.Y = clipCoords.Y / clipCoords.W;
	

    return true;
}

bool WorldToScreenEsp(const Vec3 VecOrigin, Vec2& VecScreen, float* Matrix, int ScreenWidth, int ScreenHeight) {
    Vec4 clipCoords = {};

    // Transform world coordinates to clip space
    clipCoords.X = VecOrigin.X * Matrix[0] + VecOrigin.Y * Matrix[4] + VecOrigin.Z * Matrix[8] + Matrix[12];
    clipCoords.Y = VecOrigin.X * Matrix[1] + VecOrigin.Y * Matrix[5] + VecOrigin.Z * Matrix[9] + Matrix[13];
    clipCoords.Z = VecOrigin.X * Matrix[2] + VecOrigin.Y * Matrix[6] + VecOrigin.Z * Matrix[10] + Matrix[14];
    clipCoords.W = VecOrigin.X * Matrix[3] + VecOrigin.Y * Matrix[7] + VecOrigin.Z * Matrix[11] + Matrix[15];

    // Check if object is behind the camera
    if (clipCoords.W < 0.1f) {
        return false;
    }

    // Perform perspective division to get Normalized Device Coordinates (NDC)
    Vec3 NDC;
    NDC.X = clipCoords.X / clipCoords.W;
    NDC.Y = clipCoords.Y / clipCoords.W;
    NDC.Z = clipCoords.Z / clipCoords.W;  // Useful if you want depth testing

    // Convert NDC (-1 to 1) to screen coordinates (DirectX)
    VecScreen.X = (NDC.X + 1.0f) * 0.5f * ScreenWidth;
    VecScreen.Y = (1.0f - NDC.Y) * 0.5f * ScreenHeight;  // Invert Y for DirectX

    return true;
}

