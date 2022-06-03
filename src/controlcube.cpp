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

    cube.Create_Cube(-50.0, -50.0, -50.0, 50.0, 50.0, 50.0);
    double x = 0.0;
    double y = 0.0;
    double mag = 1.0;
    

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

        // WASD:       rotate cube
        // ARROW KEYS: change view
        // Z AND X:    zoom in and out

        // ROTATE 
        if (GetAsyncKeyState('W') & 0x8000) {
            cube.RotateX(0.02 * deltaTime);
        }

        if (GetAsyncKeyState('S') & 0x8000) {
            cube.RotateX(-0.02 * deltaTime);
        }

        if (GetAsyncKeyState('A') & 0x8000) {
            cube.RotateY(-0.02 * deltaTime);
        }

        if (GetAsyncKeyState('D') & 0x8000) {
            cube.RotateY(0.02 * deltaTime);
        }


        // MOVE
        if (GetAsyncKeyState(VK_UP)) {
            y += 0.3;
        }

        if (GetAsyncKeyState(VK_DOWN)) {
            y -= 0.3;
        }

        if (GetAsyncKeyState(VK_LEFT)) {
            x += 0.3;
        }

        if (GetAsyncKeyState(VK_RIGHT)) {
            x -= 0.3;
        }


        // ZOOM 
        if (GetAsyncKeyState('Z') & 0x8000) {
            mag += 0.001;
        }

        if (GetAsyncKeyState('X') & 0x8000) {
            mag -= 0.001;
        }

        cube.SetCamera(x, y);
        cube.SetZoom(mag);
        canvas.PushCube2D(cube.GetPoints2D());
        canvas.Update();
    }
    return 0;
}
