#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include "vec.h"

#define HEIGHT 600
#define WIDTH  900


template <class DERIVED_TYPE> 
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) { }

    BOOL Create(
        LPCSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = WIDTH,
        int nHeight = HEIGHT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
        )
    {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowExA(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
            );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Window() const 
    { 
        return m_hwnd; 
    }

protected:
    virtual LPCSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};




class Render3D : public BaseWindow<Render3D>
{
private:
    bool hasUpdated = false;
    std::vector<std::vector<Vector2D>> cubes;
    
public:
    LPCSTR ClassName() const 
    { 
        return "Render Class"; 
    }

    void PaintCubes(HDC hdc, COLORREF color)
    {
        if (!hasUpdated)
            return;

        SelectObject(hdc, GetStockObject(DC_PEN));
        SetDCPenColor(hdc, color);
        for (std::vector<Vector2D> cube : cubes)
        {
            // Front face (x side)
            MoveToEx(hdc, cube[0].x, cube[0].y, NULL);
            LineTo(hdc, cube[1].x, cube[1].y);
            LineTo(hdc, cube[3].x, cube[3].y);
            LineTo(hdc, cube[2].x, cube[2].y);
            LineTo(hdc, cube[0].x, cube[0].y);
            LineTo(hdc, cube[3].x, cube[3].y);
            MoveToEx(hdc, cube[1].x, cube[1].y, NULL);
            LineTo(hdc, cube[2].x, cube[2].y);

            // Back face
            MoveToEx(hdc, cube[4].x, cube[4].y, NULL);
            LineTo(hdc, cube[5].x, cube[5].y);
            LineTo(hdc, cube[7].x, cube[7].y);
            LineTo(hdc, cube[6].x, cube[6].y);
            LineTo(hdc, cube[4].x, cube[4].y);

            // Connecting lines
            MoveToEx(hdc, cube[0].x, cube[0].y, NULL);
            LineTo(hdc, cube[4].x, cube[4].y);
            MoveToEx(hdc, cube[1].x, cube[1].y, NULL);
            LineTo(hdc, cube[5].x, cube[5].y);
            MoveToEx(hdc, cube[2].x, cube[2].y, NULL);
            LineTo(hdc, cube[6].x, cube[6].y);
            MoveToEx(hdc, cube[3].x, cube[3].y, NULL);
            LineTo(hdc, cube[7].x, cube[7].y);
        }
        // Reset vector AFTER drawing
        cubes.clear();
        hasUpdated = false;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        case WM_ERASEBKGND:
            return TRUE;
        
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                RECT rect;
                GetClientRect(m_hwnd, &rect);
                // Double buffering to remove flickering
                HDC hdc = BeginPaint(m_hwnd, &ps);
                HDC back_hdc = CreateCompatibleDC(hdc);
                HBITMAP mbmp = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
                SelectObject(back_hdc, mbmp);

                FillRect(back_hdc, &rect, (HBRUSH) (COLOR_WINDOW+1));
                PaintCubes(back_hdc, RGB(0, 0, 0));
                BitBlt(hdc, 0, 0, rect.right, rect.bottom, back_hdc, 0, 0, SRCCOPY);

                EndPaint(m_hwnd, &ps);
                DeleteObject(mbmp);
                DeleteDC(back_hdc);
                DeleteDC(hdc);
            }
            return 0;

        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        }
        return TRUE;
    }

    void Update()
    {
        hasUpdated = true;
        // Repaint the window
        InvalidateRect(m_hwnd, NULL, TRUE);
    }

    void PushCube2D(std::vector<Vector2D> v)
    {
        cubes.push_back(v);
    }
    
};
