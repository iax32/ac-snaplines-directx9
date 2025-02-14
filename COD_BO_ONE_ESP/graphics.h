#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include "datatypes.h"
#include <tchar.h>
#include <iostream>  // Include for console I/O
#include <fstream>   // Include for file stream redirection
#include "graphics.h"
#include "proc.h"
#include "datatypes.h"
#include "worldToScreen.h"

struct D3DContext {
    HWND hwnd = NULL;                  // Handle to the window
    IDirect3D9* d3d = NULL;            // Pointer to the Direct3D interface
    IDirect3DDevice9* d3ddev = NULL;   // Pointer to the Direct3D device interface
};

// Function Prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // The window procedure function prototype
void initD3D(HWND hWnd, D3DContext& d3dContext);            // Function to initialize Direct3D
void renderFrame(D3DContext& d3dContext, uintptr_t& moduleBase, HANDLE hProc);         // Function to render a frame (draw to the screen)
void cleanD3D(D3DContext& d3dContext);                // Function to clean up and release Direct3D resources
void DrawTextOnRadar(IDirect3DDevice9* pDevice, const char* text, float x, float y, D3DCOLOR color);



