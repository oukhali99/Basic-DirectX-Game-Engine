#include "Graphics.h"
#include "Main.h"
#include "Cube.h"
#include "Clock.h"
#include "Mouse.h"
#include "Pyramid.h"
#include "btBulletDynamicsCommon.h"
#include "Window.h"

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
        Window(hInstance, hPrevInstance, lpCmdLine, nCmdShow, hWnd);

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
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));


        Shape* cube1 = new Cube(gfx, dynamicsWorld);
        Shape* cube2 = new Cube(gfx, dynamicsWorld);

        MSG msg = { 0 };
        while (true)
        {
            dynamicsWorld->stepSimulation(1.0f / 600.0f, 10);
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
