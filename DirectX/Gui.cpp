#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Graphics.h"
#include "Clock.h"
#include "Game.h"

void Gui::Init(HWND hWnd) {
    instance = new Gui(hWnd);
}

Gui* Gui::GetInstance() {
	return instance;
}

Gui::Gui(HWND hWnd)
    :
    hWnd(hWnd),
    showDemoWindow(false),
    backgroundColor(ImVec4(0.3f, 0.1f, 1.0f, 1.0f)),
    nearZ(Graphics::GetInstance()->GetNearZ()),
    farZ(Graphics::GetInstance()->GetFarZ())
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(
        Graphics::GetInstance()->GetDevice(),
        Graphics::GetInstance()->GetDeviceContext()
    );
}

void Gui::Update() {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state

        ImGui::ColorEdit3("clear color", (float*)&backgroundColor);

        ImGui::SliderFloat("NearZ", &nearZ, 0.01f, farZ - 0.01f);
        ImGui::SliderFloat("FarZ", &farZ, nearZ + 0.01f, 100.0f);

        ImGui::SliderFloat("CameraZ", &cameraZ, 0, 100);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Application lifetime: %.1fs", Clock::GetSingleton().GetTimeSinceStart());
        ImGui::End();
    }

    if (nearZ != Graphics::GetInstance()->GetNearZ()) {
        Graphics::GetInstance()->SetNearZ(nearZ);
    }
    if (farZ != Graphics::GetInstance()->GetFarZ()) {
        Graphics::GetInstance()->SetFarZ(farZ);
    }

    btTransform currentCameraTransform = Game::GetInstance()->GetMainCamera()->GetGameObject()->GetTransform();
    if (cameraZ != currentCameraTransform.getOrigin().z()) {
        btVector3 newPos = currentCameraTransform.getOrigin();
        newPos.setZ(cameraZ);
        currentCameraTransform.setOrigin(newPos);
        Game::GetInstance()->GetMainCamera()->GetGameObject()->SetTransform(currentCameraTransform);
    }
}

ImVec4 Gui::GetBackgroundColor() {
    return backgroundColor;
}
