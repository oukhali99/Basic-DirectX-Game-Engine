#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <comdef.h>
#include <atlstr.h>
#include <string>
#include <chrono>
#include <sstream>

#include "Graphics.h"
#include "Main.h"

// global declarations
float xTranslation, yTranslation;

// function prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    try {
        HWND hWnd;

        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        //RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };    // set the size, but not the position
        //AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(
            NULL,
            L"WindowClass1",    // name of the window class
            L"Our First Windowed Program",   // title of the window
            WS_OVERLAPPEDWINDOW,    // window style
            300,    // x-position of the window
            100,    // y-position of the window
            SCREEN_WIDTH,    // width of the window
            SCREEN_HEIGHT,    // height of the window
            NULL,    // we have no parent window, NULL
            NULL,    // we aren't using menus, NULL
            hInstance,    // application handle
            NULL);    // used with multiple windows, NULL

        if (hWnd == nullptr) {
            throw new std::exception("hWnd was found to be null");
        }

        // display the window on the screen
        ShowWindow(hWnd, nCmdShow);

        unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
        };

        // set up and initialize Direct3D
        Graphics gfx(hWnd, indices, sizeof(indices) / sizeof(unsigned short));

        // this struct holds Windows event messages
        MSG msg = { 0 };

        // wait for the next message in the queue, store the result in 'msg'
        auto startTime = std::chrono::steady_clock::now();
        while (true)
        {
            if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
            }
            else {
                const float t = std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();
                std::ostringstream oss;
                oss << "Time elapsed: " << std::fixed << t << "s";
                SetWindowTextA(hWnd, oss.str().c_str());

                gfx.RenderFrame(t, xTranslation, yTranslation);
            }
        }

        if (msg.wParam < 0) {
            throw new std::exception((const char*)GetLastError());
        }

        // return this part of the WM_QUIT message to Windows
        return msg.wParam;
    }
    catch (std::exception* e) {
        MessageBoxA(NULL, e->what(), "Error", 0u);
        return -1;
    }
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
    case WM_DESTROY:
        // close the application entirely
        PostQuitMessage(0);
        break;
    case WM_CHAR:
        if (wParam == 'w') {
            yTranslation += 1.0f / 5.0f;
        }
        else if (wParam == 's') {
            yTranslation -= 1.0f / 5.0f;
        }
        else if (wParam == 'a') {
            xTranslation -= 1.0f / 5.0f;
        }
        else if (wParam == 'd') {
            xTranslation += 1.0f / 5.0f;
        }
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
