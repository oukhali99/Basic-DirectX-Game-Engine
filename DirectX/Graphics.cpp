#include <vector>
#include "Graphics.h"
#include "Mouse.h"
#include "Game.h"
#include "GameObject.h"
#include "Gui.h"

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
    renderTargetView->Release();
    pDSView->Release();
}

// this function initializes and prepares Direct3D for use
void Graphics::InitD3D()
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1u;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = clientRect.right;                    // set the back buffer width
    scd.BufferDesc.Height = clientRect.bottom;                  // set the back buffer height
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
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView));
    pBackBuffer->Release();

    // Create the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = clientRect.right;
    viewport.Height = clientRect.bottom;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    // Set the viewport
    pContext->RSSetViewports(1, &viewport);

    // select which primtive type we are using
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::InitPipeline() {
    LPCWSTR shaderFiles[] = { SHADER_FILE_NAME_DEFAULT, SHADER_FILE_NAME_TEXTURE };

    ID3DBlob* VS = NULL;
    ID3DBlob* PS = NULL;
    for (LPCWSTR shaderFile : shaderFiles) {
        // load and compile the two shaders
        ID3DBlob* errorBlob = NULL;
        ID3D11VertexShader* pVS;
        ID3D11PixelShader* pPS;

        hr = D3DCompileFromFile(shaderFile, 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &errorBlob);
        if (errorBlob) {
            LPCSTR message = (LPCSTR)errorBlob->GetBufferPointer();
            Main::HandleError(hr, __FILE__, __LINE__, message);
        }

        hr = D3DCompileFromFile(shaderFile, 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &errorBlob);
        if (errorBlob) {
            LPCSTR message = (LPCSTR)errorBlob->GetBufferPointer();
            Main::HandleError(hr, __FILE__, __LINE__, message);
        }

        GFX_THROW_INFO(pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS));
        GFX_THROW_INFO(pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS));

        std::pair<ID3D11VertexShader*, ID3D11PixelShader*> shaderPair;
        shaderPair.first = pVS;
        shaderPair.second = pPS;

        compiledShaders[shaderFile] = shaderPair;
    }

    // set the shader objects
    SetShaders(L"DefaultShaders.shaders");

    // Create the shader layout (input)
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        {"TEXCOORDS", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, sizeof(ied) / sizeof(ied[0]),
        VS->GetBufferPointer(),
        VS->GetBufferSize(),
        &pLayout)
    );
    pContext->IASetInputLayout(pLayout);

    PS->Release();
    VS->Release();
}

void Graphics::InitGraphics() {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

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
    td.Width = clientRect.right;
    td.Height = clientRect.bottom;
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
    pContext->OMSetRenderTargets(1u, &renderTargetView, pDSView);
}

ID3D11Device* Graphics::GetDevice() {
    return pDevice;
}

ID3D11DeviceContext* Graphics::GetDeviceContext() {
    return pContext;
}

void Graphics::SetShaders(LPCWSTR shaderFileName) {
    pContext->VSSetShader(compiledShaders[shaderFileName].first, 0, 0);
    pContext->PSSetShader(compiledShaders[shaderFileName].second, 0, 0);
}

void Graphics::ClearFrame() {
    // clear the back buffer to a deep blue
    Gui* gui = Gui::GetInstance();
    ImVec4 color = gui->GetBackgroundColor();
    float colorFloat[4] = { color.x, color.y, color.z, color.w };
    pContext->ClearRenderTargetView(renderTargetView, colorFloat);
    pContext->ClearDepthStencilView(pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

// this is the function used to render a single frame
void Graphics::RenderFrame()
{
    // switch the back buffer and the front buffer
    GFX_THROW_INFO(swapchain->Present(0u, 0u));
}
