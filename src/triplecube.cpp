#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include "include/cube.h"
#include "include/render.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    Render3D canvas;
    Cube     cube;
    Cube     cube2;
    Cube     cube3;

    cube.Create_Cube(-50.0, -50.0, -50.0, 50.0, 50.0, 50.0);
    cube.SetCamera(200.0, 0.0);
    cube2.Create_Cube(-50.0, -50.0, -50.0, 50.0, 50.0, 50.0);
    cube2.SetCamera(-200.0, -75.0);
    cube3.Create_Cube(-50.0, -50.0, -50.0, 50.0, 50.0, 50.0);
    cube3.SetCamera(-50.0, 120.0);

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

        // Rotate cubes in different axes
        cube.RotateY(0.01 * deltaTime);
        cube2.RotateX(0.01 * deltaTime);
        cube3.RotateY(0.02 * deltaTime);

        canvas.PushCube(cube.GetPoints2D());
        canvas.PushCube(cube2.GetPoints2D());
        canvas.PushCube(cube3.GetPoints2D());

        canvas.Update();
    }
    return 0;
}
