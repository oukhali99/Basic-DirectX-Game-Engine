#include "Window.h"
#include "Main.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Window::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    instance = new Window(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

Window* Window::GetInstance() {
    return instance;
}

HWND Window::GetHandle() {
    return hWnd;
}

Window::Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
    ZeroMemory(&hWnd, sizeof(hWnd));
    hWnd = CreateWindowEx(
        NULL,
        L"WindowClass1",    // name of the window class
        L"Our First Windowed Program",   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        0,    // x-position of the window
        0,    // y-position of the window
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

    // Register the mouse for raw input
    RAWINPUTDEVICE rid;
    ZeroMemory(&rid, sizeof(rid));
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = NULL;
    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
        throw new std::exception("Failed to create rawinput device");
    }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    // sort through and find what code to run for the message given
    switch (message)
    {
    case WM_DESTROY:
        ClipCursor(NULL);

        // close the application entirely
        PostQuitMessage(0);
        break;
    case WM_ACTIVATE:
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        MapWindowPoints(hWnd, NULL, reinterpret_cast<POINT*>(&clientRect), 2);
        ClipCursor(&clientRect);
        break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
