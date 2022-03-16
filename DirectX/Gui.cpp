#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Graphics.h"

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
    showAnotherWindow(false),
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
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &showAnotherWindow);

        ImGui::ColorEdit3("clear color", (float*)&backgroundColor);

        ImGui::SliderFloat("NearZ", &nearZ, 0.01f, farZ - 0.01f);
        ImGui::SliderFloat("FarZ", &farZ, nearZ + 0.01f, 100.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (nearZ != Graphics::GetInstance()->GetNearZ()) {
        Graphics::GetInstance()->SetNearZ(nearZ);
    }
    if (farZ != Graphics::GetInstance()->GetFarZ()) {
        Graphics::GetInstance()->SetFarZ(farZ);
    }

    // 3. Show another simple window.
    if (showAnotherWindow)
    {
        ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            showAnotherWindow = false;
        ImGui::End();
    }
}

ImVec4 Gui::GetBackgroundColor() {
    return backgroundColor;
}
