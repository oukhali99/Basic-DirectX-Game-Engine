#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
    : 
    indices(indices),
    indexCount(indexCount),
    hWnd(hWnd)
{
    InitD3D();
    InitPipeline();
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
}


ID3D11Device* Graphics::GetPDevice() {
    return pDevice;
}

ID3D11DeviceContext* Graphics::GetPContext() {
    return pContext;
}


void Graphics::AddShape(Shape* shape) {
    shapes.push_back(shape);
}


// this function initializes and prepares Direct3D for use
void Graphics::InitD3D()
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
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

    // set the render target as the back buffer
    pContext->OMSetRenderTargets(1, &backbuffer, NULL);

    // Create the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    // Set the viewport
    pContext->RSSetViewports(1, &viewport);
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

// this is the function used to render a single frame
void Graphics::RenderFrame()
{
    // clear the back buffer to a deep blue
    float color[4] = { 0.3f, 0.1f, 1.0f, 1.0f };
    pContext->ClearRenderTargetView(backbuffer, color);

    // Render all the shapes
    for (Shape* shape : shapes) {
        shape->RenderFrame();
    }

    // switch the back buffer and the front buffer
    GFX_THROW_INFO(swapchain->Present(0u, 0u));
}

void Graphics::ButtonPressed(WPARAM wParam) {
    for (Shape* shape : shapes) {
        shape->ButtonPressed(wParam);
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
