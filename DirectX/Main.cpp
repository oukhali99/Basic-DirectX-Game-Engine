#include "Main.h"
#include "Cube.h"
#include "Clock.h"
#include "Mouse.h"
#include "Pyramid.h"
#include "btBulletDynamicsCommon.h"
#include "Window.h"
#include "Physics.h"
#include "GameObject.h"
#include "Game.h"

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

        Game::Init(hWnd);
        Physics::Init();
        Graphics::Init(hWnd, 0.5f, 50.0f);

        btTransform transform;
        btVector3 size;
        btScalar mass;

        transform.setIdentity();
        transform.setOrigin(btVector3(12, 8, 20));
        size = btVector3(2, 2, 1);
        mass = 1;

        GameObject* object1 = new GameObject(transform, size, mass);
        Shape* cube1 = new Cube();
        object1->AddShape(cube1);
        object1->AddRigidbody();

        transform.setIdentity();
        transform.setOrigin(btVector3(-12, 8, 20));
        GameObject* object2 = new GameObject(transform, size, mass);
        Shape* cube2 = new Cube();
        object2->AddShape(cube2);
        object2->AddRigidbody();

        transform.setIdentity();
        transform.setOrigin(btVector3(0, 10, 20));
        GameObject* object3 = new GameObject(transform, size, mass);
        Shape* cube3 = new Cube();
        object3->AddShape(cube3);
        object3->AddRigidbody();

        transform.setIdentity();
        transform.setOrigin(btVector3(0, -8, 20));
        size = btVector3(15, 1, 1);
        mass = 0;

        GameObject* floor = new GameObject(transform, size, mass);
        Shape* floorShape = new Cube();
        floor->AddShape(floorShape);
        floor->AddRigidbody();

        MSG msg = { 0 };
        while (true)
        {
            Game::GetInstance()->Update();

            if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
                else if (msg.message == WM_CHAR) {
                    // TODO
                }
            }
            else {
                const float t = Clock::GetSingleton().GetTimeSinceStart();
                const Mouse::Position mp = Mouse::GetSingleton(hWnd).GetPosition();

                std::ostringstream oss;
                //oss << "Time elapsed: " << std::fixed << t << "s";
                oss << "Mouse Position: " << std::fixed << "(" << mp.x << ", " << mp.y << ")";
                SetWindowTextA(hWnd, oss.str().c_str());
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
