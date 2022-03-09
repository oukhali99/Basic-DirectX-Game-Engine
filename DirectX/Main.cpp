#include "Graphics.h"
#include "Main.h"
#include "Cube.h"
#include "Clock.h"
#include "Mouse.h"
#include "Pyramid.h"
#include "btBulletDynamicsCommon.h"

Graphics* gfx;

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

        // set up and initialize Direct3D
        gfx = new Graphics(hWnd);

        // Create the configuration
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

        // Create the collision dispatcher
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

        // ???
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

        // Constraint solver
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

        // Create the world
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
            overlappingPairCache, solver, collisionConfiguration);

        // Set the gravity
        dynamicsWorld->setGravity(btVector3(0, -0.5f, 0));

        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(-2, 0, 10));
        transform.setRotation(btQuaternion(90, 90, 90, 0));
        Shape* cube1 = new Cube(*gfx, dynamicsWorld, transform);

        transform.setIdentity();
        transform.setOrigin(btVector3(2, 0, 10));
        Shape* cube2 = new Cube(*gfx, dynamicsWorld, transform);

        // this struct holds Windows event messages
        MSG msg = { 0 };

        // wait for the next message in the queue, store the result in 'msg'
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
                const float t = Clock::GetSingleton().GetTimeSinceStart();

                const Mouse::Position mp = Mouse::GetSingleton(hWnd).GetPosition();

                std::ostringstream oss;
                //oss << "Time elapsed: " << std::fixed << t << "s";
                oss << "Mouse Position: " << std::fixed << "(" << mp.x << ", " << mp.y << ")";
                SetWindowTextA(hWnd, oss.str().c_str());

                dynamicsWorld->stepSimulation(1.0f / 600.0f, 10);
                gfx->RenderFrame();
            }
        }

        delete gfx;

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
        gfx->ButtonPressed(wParam);
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
