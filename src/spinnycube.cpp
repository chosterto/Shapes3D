#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include "include/cube.h"
#include "include/render.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    Render3D canvas;
    Cube cube;

    cube.Create_Cube(-50.0, -50.0, -50.0, 50.0, 50.0, 50.0);
    double x = 0.0;
    double y = 0.0;


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
        // Rotate in all axes and move camera
        cube.RotateX(0.02 * deltaTime);
        cube.RotateY(0.01 * deltaTime);
        cube.RotateZ(0.03 * deltaTime);
        cube.SetCamera(100.0 * cos(x), 100.0 * sin(y));
        cube.SetZoom(sin(x) + 1.2);
        x += 0.01 * deltaTime;
        y += 0.01 * deltaTime;

        canvas.PushCube2D(cube.GetPoints2D());
        canvas.Update();
    }
    return 0;
}
