// build command: g++ src/test.cpp -lgdi32

#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include "include/cube.h"
#include "include/render.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    Render3D canvas;

    if (!canvas.Create("3D Rendering", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(canvas.Window(), nCmdShow);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    
    MSG msg = { };
    while (true)
    {
        t2 = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(t2 - t1).count();
        deltaTime = deltaTime * 80.0;
        t1 = t2;
        
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return TRUE;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // SOMETHING GOES HERE!!!
        canvas.Update();
    }
    return 0;
}
