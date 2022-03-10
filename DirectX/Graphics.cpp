#include <vector>

#include "Graphics.h"
#include "Mouse.h"
#include "Game.h"
#include "GameObject.h"

void Graphics::Init(HWND hWnd, float nearZ, float farZ) {
    instance = new Graphics(hWnd, nearZ, farZ);
}

Graphics* Graphics::GetInstance() {
    return instance;
}

Graphics::Graphics(HWND hWnd, float nearZ, float farZ)
    :
    hWnd(hWnd),
    nearZ(nearZ),
    farZ(farZ)
{
    InitD3D();
    InitPipeline();
    InitGraphics();
}

Graphics::~Graphics() {
    GFX_THROW_INFO(swapchain->SetFullscreenState(FALSE, NULL));

    // close and release all existing COM objects
    swapchain->Release();
    pDevice->Release();
    pContext->Release();
    backbuffer->Release();
    pVS->Release();
    pPS->Release();
    pDSView->Release();
}

// this function initializes and prepares Direct3D for use
void Graphics::InitD3D()
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1u;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4u;                              // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // create a device, device context and swap chain using the information in the scd struct
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &pDevice,
        NULL,
        &pContext
    ));

    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    GFX_THROW_INFO(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

    // use the back buffer address to create the render target
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer));
    pBackBuffer->Release();

    // Create the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    // Set the viewport
    pContext->RSSetViewports(1, &viewport);

    // select which primtive type we are using
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::InitPipeline() {
    // load and compile the two shaders
    ID3DBlob* VS = NULL;
    ID3DBlob* PS = NULL;
    ID3DBlob* errorBlob = NULL;

    GFX_THROW_INFO(D3DCompileFromFile(L"Shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &errorBlob));
    GFX_THROW_INFO(D3DCompileFromFile(L"Shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &errorBlob));

    GFX_THROW_INFO(pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS));
    GFX_THROW_INFO(pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS));

    // set the shader objects
    pContext->VSSetShader(pVS, 0, 0);
    pContext->PSSetShader(pPS, 0, 0);

    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, sizeof(ied) / sizeof(ied[0]),
        VS->GetBufferPointer(),
        VS->GetBufferSize(),
        &pLayout)
    );
    pContext->IASetInputLayout(pLayout);    
}

void Graphics::InitGraphics() {
    // Create the depth stencil state
    ID3D11DepthStencilState* pDSState;
    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory(&dsd, sizeof(dsd));
    dsd.DepthEnable = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsd, &pDSState));
    pContext->OMSetDepthStencilState(pDSState, 1u);

    // Create the depth stencil texture
    ID3D11Texture2D* pDepthStencilTexture;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = SCREEN_WIDTH;
    td.Height = SCREEN_HEIGHT;
    td.MipLevels = 1u;
    td.ArraySize = 1u;
    td.Format = DXGI_FORMAT_D32_FLOAT;                      // Shape::Transform.z is a float (32bit)
    td.SampleDesc.Count = 4u;
    td.SampleDesc.Quality = 0u;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(pDevice->CreateTexture2D(&td, NULL, &pDepthStencilTexture));

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    dsvd.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencilTexture, &dsvd, &pDSView));

    pContext->OMSetRenderTargets(1u, &backbuffer, pDSView);
}

// this is the function used to render a single frame
void Graphics::RenderFrame()
{
    // clear the back buffer to a deep blue
    float color[4] = { 0.3f, 0.1f, 1.0f, 1.0f };
    pContext->ClearRenderTargetView(backbuffer, color);
    pContext->ClearDepthStencilView(pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0u);

    // Render all the shapes
    std::vector<GameObject*> gameObjects = Game::GetInstance()->GetGameObjects();
    for (GameObject* gameObject : gameObjects) {
        gameObject->RenderFrame();
    }

    // switch the back buffer and the front buffer
    GFX_THROW_INFO(swapchain->Present(0u, 0u));
}

void Graphics::ButtonPressed(WPARAM wParam) {
    std::vector<GameObject*> gameObjects = Game::GetInstance()->GetGameObjects();
    for (GameObject* gameObject : gameObjects) {
        Shape* shape = gameObject->GetShape();
        if (shape) {
            shape->ButtonPressed(wParam);
        }        
    }
}

void Graphics::HandleError(HRESULT hr, const char* file, const long long line) {
    std::stringstream ss;

    // Exception line
    ss << "Exception on line " << line << std::endl;

    // Exception file
    ss << "File: " << file << std::endl;

    // Error code
    ss << "Error code: " << "0x" << std::hex << hr << std::endl;

    // Error description
    char errorMessage[50];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorMessage,
        sizeof(errorMessage) / sizeof(errorMessage[0]),
        NULL
    );

    ss << "Description: " << errorMessage << std::endl;

    throw new std::exception(ss.str().c_str());
}
