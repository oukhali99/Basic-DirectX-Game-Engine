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
#include "Rigidbody.h"
#include "Shape.h"

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

        transform.setIdentity();
        transform.setOrigin(btVector3(12, 8, 20));
        size = btVector3(2, 2, 1);


        GameObject* object1 = new GameObject(transform, size);

        object1->AddComponent<Cube>();
        Shape* shape1 = object1->GetComponent<Shape>();
        shape1->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

        object1->AddComponent<Rigidbody>();
        Rigidbody* rb = object1->GetComponent<Rigidbody>();
        rb->SetMass(1);


        transform.setIdentity();
        transform.setOrigin(btVector3(-12, 8, 20));

        GameObject* object2 = new GameObject(transform, size);

        object2->AddComponent<Cube>();


        transform.setIdentity();
        transform.setOrigin(btVector3(0, 8, 20));

        GameObject* object3 = new GameObject(transform, size);

        object3->AddComponent<Cube>();


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
