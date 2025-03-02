#include "Main.h"
#include "Cube.h"
#include "Clock.h"
#include "Mouse.h"
#include "Pyramid.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "Window.h"
#include "Physics.h"
#include "GameObject.h"
#include "Game.h"
#include "Rigidbody.h"
#include "Shape.h"
#include "Script.h"
#include "Keyboard.h"
#include "Texture.h"
#include "Gui.h"
#include "Camera.h"
#include "PositionConstraint.h"
#include "Wedge.h"
#include "Light.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    try {
        Window::Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
        HWND hWnd = Window::GetInstance()->GetHandle();
        Game::Init(hWnd);
        Physics::Init();
        Graphics::Init(hWnd, 0.5f, 50.0f);
        Gui::Init(hWnd);

        Mouse::Init(hWnd);

        FaceColor faceColors[] = {
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 0.0f, 1.0f },
        };

        // Camera
        {
            btVector3 size(1, 1, 1);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0.5f, 0));

            GameObject* camera = new GameObject(transform, size);

            camera->AddComponent<Camera>();
            Camera* cameraComponent = camera->GetComponent<Camera>();
            Game::GetInstance()->SetMainCamera(cameraComponent);

            //camera->AddComponent<Light>();
            //Light* light = camera->GetComponent<Light>();
            //light->lightData.SetDiffuseIntensity(1);
            
            camera->AddComponent<Script>();
            Script* script = camera->GetComponent<Script>();
            btScalar yaw = 0, pitch = 0;
            script->SetOnUpdate([&yaw, &pitch](GameObject* gameObject) {
                float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                btTransform oldTransform = gameObject->GetTransform();
                btTransform newTransform = oldTransform;

                // Hide cursor
                ShowCursor(false);

                // Rotation
                Mouse::RawInput* mouseRawInput = Mouse::GetInstance()->GetRawInput();
                btScalar torqueMagnitude = 2.0f * deltaTime;

                yaw += torqueMagnitude * mouseRawInput->x;
                pitch += torqueMagnitude * mouseRawInput->y;

                btQuaternion newRotation(yaw, pitch, 0);
                newTransform.setRotation(newRotation);                

                gameObject->SetTransform(newTransform);
            });

            // Player body
            {
                btVector3 size(0.8f, 1, 0.8f);

                btTransform transform;
                transform.setIdentity();
                transform.setOrigin(btVector3(0, 0, 0));

                GameObject* player = new GameObject(transform, size);

                player->AddComponent<Rigidbody>();
                Rigidbody* rb = player->GetComponent<Rigidbody>();
                rb->SetMass(1);
                rb->SetIsKinematic(false);
                rb->SetAngularFactor(btVector3(0, 1, 0));

                // Add constraint to camera
                camera->AddComponent<PositionConstraint>();
                PositionConstraint* positionConstraint = camera->GetComponent<PositionConstraint>();
                positionConstraint->SetConstrainer(player);

                player->AddComponent<Script>();
                player->GetComponent<Script>()->SetOnUpdate([rb, &yaw, &pitch](GameObject* gameObject) {
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                    btTransform oldTransform = gameObject->GetTransform();

                    // Translation
                    btVector3 translation(0, 0, 0);
                    btScalar translationMagnitude = 1000 * deltaTime;

                    for (char key : *Keyboard::GetInstance()->GetPressedKeys()) {
                        switch (key) {
                        case 'W':
                            translation.setZ(1);
                            break;
                        case 'S':
                            translation.setZ(-1);
                            break;
                        case 'D':
                            translation.setX(1);
                            break;
                        case 'A':
                            translation.setX(-1);
                            break;
                        case 'E':
                            translation.setY(1);
                            break;
                        case 'Q':
                            translation.setY(-1);
                            break;
                        }
                    }

                    // Make into unit
                    if (!translation.isZero()) {
                        translation = translation.normalize();
                    }

                    // Adjust for rotation
                    btQuaternion newRotation(yaw, 0, 0);
                    translation = translation.rotate(
                        newRotation.getAxis(),
                        newRotation.getAngle()
                    );

                    // Set magnitude
                    translation *= translationMagnitude;

                    // Set speed cap
                    if (rb->GetLinearVelocity().norm() < 4) {
                        rb->ApplyImpulse(translation);
                    }

                    // Friction
                    btVector3 friction = -0.3f * rb->GetLinearVelocity();
                    friction.setY(0);
                    rb->ApplyImpulse(friction);
                });
            }
        }

        // Ground
        {
            btVector3 size(10, 1, 10);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, -2, 0));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("grass.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);

            object->AddComponent<Script>();
            Script* script = object->GetComponent<Script>();
            script->SetOnUpdate([rb](GameObject* gameObject) {
                for (WPARAM wParam : *Keyboard::GetInstance()->GetPressedKeys()) {
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                    btVector3 unitTorque(0, 0, 0);
                    btScalar torqueMagnitude = 2.0f * deltaTime;

                    if (wParam == 'W') {
                        unitTorque.setY(1);
                    }
                    else if (wParam == 'S') {
                        unitTorque.setY(-1);
                    }
                    else if (wParam == 'D') {
                        unitTorque.setX(1);
                    }
                    else if (wParam == 'A') {
                        unitTorque.setX(-1);
                    }

                    //rb->ApplyImpulse(unitTorque * torqueMagnitude);
                }
                });
        }

        // Ceiling
        {
            btVector3 size(10, 1, 10);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 4, 0));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);
        }


        // Wedge
        {
            btVector3 size(1, 1, 1);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0, 5));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Wedge>();
            Shape* shape = object->GetComponent<Shape>();
            shape->SetFaceColors(faceColors);

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(1);
            rb->SetIsKinematic(false);

            object->AddComponent<Script>();
            Script* script = object->GetComponent<Script>();
            script->SetOnUpdate([rb](GameObject* gameObject) {
                for (WPARAM wParam : *Keyboard::GetInstance()->GetPressedKeys()) {
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                    btVector3 unitTorque(0, 0, 0);
                    btScalar torqueMagnitude = 2.0f * deltaTime;

                    if (wParam == 'W') {
                        unitTorque.setY(1);
                    }
                    else if (wParam == 'S') {
                        unitTorque.setY(-1);
                    }
                    else if (wParam == 'D') {
                        unitTorque.setX(1);
                    }
                    else if (wParam == 'A') {
                        unitTorque.setX(-1);
                    }

                    //rb->ApplyImpulse(unitTorque * torqueMagnitude);
                }
                });


            object->AddComponent<Light>();
        }

        // Cube
        {
            btVector3 size(0.1f, 0.1f, 0.1f);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0, -5));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);

            object->AddComponent<Light>();

            object->AddComponent<Script>();
            Script* script = object->GetComponent<Script>();
            script->SetOnUpdate([rb](GameObject* gameObject) {
                for (WPARAM wParam : *Keyboard::GetInstance()->GetPressedKeys()) {
                    float deltaTime = Clock::GetSingleton().GetTimeSinceStart() - Game::GetInstance()->GetLastUpdateTime();
                    btVector3 unitTorque(0, 0, 0);
                    btScalar torqueMagnitude = 2.0f * deltaTime;

                    if (wParam == 'W') {
                        unitTorque.setY(1);
                    }
                    else if (wParam == 'S') {
                        unitTorque.setY(-1);
                    }
                    else if (wParam == 'D') {
                        unitTorque.setX(1);
                    }
                    else if (wParam == 'A') {
                        unitTorque.setX(-1);
                    }

                    //rb->ApplyImpulse(unitTorque * torqueMagnitude);
                }
                });
        }

        // Wall S
        {
            btVector3 size(10, 3, 1);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0, -10));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);
            shape->SetFaceColors(faceColors);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);
        }

        // Wall N
        {
            btVector3 size(10, 3, 1);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(0, 0, 10));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);
        }

        // Wall E
        {
            btVector3 size(1, 3, 10);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(10, 0, 0));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);
        }

        // Wall W
        {
            btVector3 size(1, 3, 10);

            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(-10, 0, 0));

            GameObject* object = new GameObject(transform, size);

            object->AddComponent<Cube>();
            Shape* shape = object->GetComponent<Shape>();

            Texture* texture = new Texture("brick.jpg");
            shape->SetTexture(texture);

            object->AddComponent<Rigidbody>();
            Rigidbody* rb = object->GetComponent<Rigidbody>();
            rb->SetMass(0);
            rb->SetIsKinematic(true);
        }

        MSG msg = { 0 };
        std::vector<BYTE> rawBuffer;
        while (true)
        {
            const float t = Clock::GetSingleton().GetTimeSinceStart();

            std::ostringstream oss;
            oss << "Time elapsed: " << std::fixed << t << "s";
            //oss << "Mouse Position: " << std::fixed << "(" << Mouse::GetInstance()->GetRawInput()->x << ", " << Mouse::GetInstance()->GetRawInput()->y << ")";
            SetWindowTextA(hWnd, oss.str().c_str());

            Game::GetInstance()->Update();

            // Reset the mouse rawinputstd::ostringstream oss;
            Mouse::GetInstance()->SetRawInput(0, 0);

            while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                std::stringstream ss;

                switch (msg.message) {
                case WM_QUIT:
                    goto mainLoopExit;
                    break;
                case WM_KEYDOWN:
                    //ss << "User pressed: ";
                    //ss << ((HIWORD(msg.lParam) & KF_REPEAT) == KF_REPEAT) << std::endl;
                    //OutputDebugStringA(ss.str().c_str());

                    if (ImGui::GetIO().WantCaptureKeyboard)
                    {
                        break;
                    }

                    if ((HIWORD(msg.lParam) & KF_REPEAT) != KF_REPEAT) {
                        Keyboard::GetInstance()->InputStarted(msg.wParam);
                    }
                    break;
                case WM_KEYUP:
                    Keyboard::GetInstance()->InputStopped(msg.wParam);
                    break;
                case WM_INPUT:
                    UINT size = 0u;
                    // first get the size of the input data
                    if (GetRawInputData(
                        reinterpret_cast<HRAWINPUT>(msg.lParam),
                        RID_INPUT,
                        nullptr,
                        &size,
                        sizeof(RAWINPUTHEADER)) == -1
                    ) {
                        // bail msg processing if error
                        break;
                    }
                    rawBuffer.resize(size);

                    // read in the input data
                    if (GetRawInputData(
                        reinterpret_cast<HRAWINPUT>(msg.lParam),
                        RID_INPUT,
                        rawBuffer.data(),
                        &size,
                        sizeof(RAWINPUTHEADER)) != size
                    )  {
                        // bail msg processing if error
                        break;
                    }

                    auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
                    if (ri.header.dwType == RIM_TYPEMOUSE) {
                        Mouse::GetInstance()->OnRawInput(ri.data.mouse.lLastX, ri.data.mouse.lLastY);

                        std::ostringstream oss;
                        oss << "User moved mouse: ";
                        oss << Mouse::GetInstance()->GetRawInput()->x << ", " << Mouse::GetInstance()->GetRawInput()->y << std::endl;
                        //SetWindowTextA(hWnd, oss.str().c_str());
                    }


                    //std::ostringstream oss;
                    //oss << "Time elapsed: " << std::fixed << t << "s";
                    //oss << "Mouse Position: " << std::fixed << "(" << mp.x << ", " << mp.y << ")";

                    break;
                }
            }
        }

        mainLoopExit:
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
