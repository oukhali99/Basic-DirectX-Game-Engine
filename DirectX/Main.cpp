#include "Graphics.h"
#include "Main.h"
#include "Cube.h"
#include "Clock.h"
#include "Mouse.h"
#include "Pyramid.h"
#include "btBulletDynamicsCommon.h"
#include "Window.h"
#include "Physics.h"

Graphics* gfx;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    try {
        HWND hWnd;
        ZeroMemory(&hWnd, sizeof(hWnd));
        Window window(hInstance, hPrevInstance, lpCmdLine, nCmdShow, hWnd);

        gfx = new Graphics(hWnd, 0.5f, 50.0f);

        btDiscreteDynamicsWorld* dynamicsWorld;
        Physics physics(&dynamicsWorld);

        Shape* cube1 = new Cube(gfx, dynamicsWorld);
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(0, 20, 40));
        cube1->SetTransform(transform);
        cube1->AddRigidBody();
        cube1->followKeyboard = true;

        Shape* floor = new Cube(gfx, dynamicsWorld);
        transform.setIdentity();
        transform.setOrigin(btVector3(0, -20, 40));
        floor->SetTransform(transform);
        floor->AddRigidBody();
        floor->SetMass(0);

        MSG msg = { 0 };
        float last = Clock::GetSingleton().GetTimeSinceStart();
        while (true)
        {
            dynamicsWorld->stepSimulation(Clock::GetSingleton().GetTimeSinceStart() - last, 10);
            last = Clock::GetSingleton().GetTimeSinceStart();

            if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
                else if (msg.message == WM_CHAR) {
                    gfx->ButtonPressed(msg.wParam);
                }
            }
            else {
                const float t = Clock::GetSingleton().GetTimeSinceStart();

                const Mouse::Position mp = Mouse::GetSingleton(hWnd).GetPosition();

                std::ostringstream oss;
                //oss << "Time elapsed: " << std::fixed << t << "s";
                oss << "Mouse Position: " << std::fixed << "(" << mp.x << ", " << mp.y << ")";
                SetWindowTextA(hWnd, oss.str().c_str());

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
