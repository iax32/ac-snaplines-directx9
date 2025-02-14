#include <windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <iostream>  // Include for console I/O
#include <fstream>   // Include for file stream redirection
#include "graphics.h"
#include "proc.h"
#include "datatypes.h"
#include "worldToScreen.h"

D3DContext d3dContext;



// Main Entry Point of the Program
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Get the process ID and module base address
    DWORD procId = GetProcId(L"ac_client.exe");
    uintptr_t moduleBase = GetModuleBaseAddr(procId, L"ac_client.exe");
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    // Allocate a console for the application
    AllocConsole();

    // Redirect standard I/O streams to the console using safer functions
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONIN$", "r", stdin);

    // Now you can use std::cout, std::cerr, and std::cin for console I/O
    std::cout << "Console initialized successfully!" << std::endl;

    
    

    // Define the Window Class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = _T("OverlayWindow");

    RegisterClassEx(&wc);

    // Create the Window
    d3dContext.hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        _T("DirectX9 Overlay"),
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL,
        hInstance, NULL);

    SetLayeredWindowAttributes(d3dContext.hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(d3dContext.hwnd, SW_SHOWDEFAULT);
    UpdateWindow(d3dContext.hwnd);

    // Initialize Direct3D
    initD3D(d3dContext.hwnd, d3dContext);

    MSG msg;
    while (TRUE)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        if (msg.message == WM_QUIT)
            break;

        
        renderFrame(d3dContext, moduleBase, hProc);

        
    }

    cleanD3D(d3dContext);
    UnregisterClass(wc.lpszClassName, hInstance);

    // Return the exit code of the program
    return msg.wParam;
}
