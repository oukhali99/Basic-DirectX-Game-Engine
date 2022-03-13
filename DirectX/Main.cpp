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
#include "Script.h"
#include "Keyboard.h"

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
        transform.setOrigin(btVector3(0, 8, 20));
        size = btVector3(2, 2, 3);

        FaceColor faceColors[] = {
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 0.0f, 1.0f },
        };

        {
            btTransform transform;
            btVector3 size;

            transform.setIdentity();
            transform.setOrigin(btVector3(0, -8, 20));
            size = btVector3(12, 1, 12);

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetFaceColors(faceColors);
            //shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);

            object->AddComponent<Script>();
            Script* inputController = object->GetComponent<Script>();
            inputController->SetOnUpdate([rb](GameObject* gameObject)->void {
                return;
                for (WPARAM wParam : *Keyboard::GetInstance()->GetPressedKeys()) {
                    char button = (char)wParam;
                    btVector3 unitImpulse(0, 0, 0);
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                    btScalar impulseMagnitude = 15.0f * deltaTime;
                    if (button == 'W') {
                        unitImpulse.setY(1);
                    }
                    if (button == 'S') {
                        unitImpulse.setY(-1);
                    }
                    if (button == 'D') {
                        unitImpulse.setX(1);
                    }
                    if (button == 'A') {
                        unitImpulse.setX(-1);
                    }
                    rb->ApplyImpulse(impulseMagnitude * unitImpulse);
                }
            });
        }
        {
            btVector3 size(2, 2, 2);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0, 20));
            //transform.setRotation(btQuaternion(0, -0.25f * 2 * 3.14, 0));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/brick.png");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);

            object->AddComponent<Script>();
            Script* script = object->GetComponent<Script>();
            script->SetOnUpdate([rb](GameObject* gameObject) {
                for (WPARAM wParam : *Keyboard::GetInstance()->GetPressedKeys()) {
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() -  Game::GetInstance()->GetLastUpdateTime();
                    btVector3 unitTorque(0, 0, 0);
                    btScalar torqueMagnitude = 2.0f * deltaTime;

                    if (wParam == 'W') {
                        unitTorque.setX(1);
                    }
                    else if (wParam == 'S') {
                        unitTorque.setX(-1);
                    }
                    else if (wParam == 'D') {
                        unitTorque.setY(-1);
                    }
                    else if (wParam == 'A') {
                        unitTorque.setY(1);
                    }
                    else if (wParam == 'Q') {
                        unitTorque.setZ(1);
                    }
                    else if (wParam == 'E') {
                        unitTorque.setZ(-1);
                    }

                    rb->ApplyTorqueImpulse(unitTorque * torqueMagnitude);
                }
            });
        }
        /*
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.png");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        {
            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetTexturePath("C:/Users/Oussama/Projects/stb/data/dog.jpg");

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
        }
        */

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
                else if (msg.message == WM_KEYDOWN) {
                    /*
                    std::stringstream ss;
                    ss << "User pressed: ";
                    ss << (char)msg.wParam << std::endl;

                    OutputDebugStringA(ss.str().c_str());
                    */
                    Keyboard::GetInstance()->InputStarted(msg.wParam);
                }
                else if (msg.message == WM_KEYUP) {
                    Keyboard::GetInstance()->InputStopped(msg.wParam);
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
