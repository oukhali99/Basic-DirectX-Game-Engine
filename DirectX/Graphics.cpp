#include "Graphics.h"

Graphics::Graphics(HWND hWnd, unsigned short* indices, unsigned short indexCount)
    : 
    indices(indices),
    indexCount(indexCount)
{
    InitD3D(hWnd);
    InitPipeline();
    InitGraphics();
}

// this function initializes and prepares Direct3D for use
void Graphics::InitD3D(HWND hWnd)
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

void Graphics::InitPipeline()
{
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
    // create a triangle using the VERTEX struct
    const VERTEX OurVertices[] = {
        // PYRAMID VERTICES
        /*
        {0.0f, 1.0f, 0.0f,
        255u, 0u, 0u, 255u},

        {0.5f, -0.5, 0.0f,
        0u, 255u, 0u, 255u},

        {-0.5f, -0.5f, 0.0f,
        0u, 0u, 255u, 255u},

        {0.0f, 0.0f, 0.6f,
        255u, 255u, 255u, 255u},
        */

        // CUBE VERTICES
        {1.0f, 1.0f, -1.0f},

        {1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},

        {-1.0f, 1.0f, -1.0f},

        {1.0f, 1.0f, 1.0f},

        {1.0f, -1.0f, 1.0f},

        {-1.0f, -1.0f, 1.0f},

        {-1.0f, 1.0f, 1.0f},
    };

    // Create a resource for the vertices
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = OurVertices;

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(OurVertices);             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pVBuffer));       // create the buffer

    // Create index resource
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = indices;

    // Create the index buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(unsigned short) * indexCount;
    bd.StructureByteStride = sizeof(unsigned short);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pIBuffer));

    // select which buffers to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, &pVBuffer, &stride, &offset);
    pContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R16_UINT, 0u);

    // Create and bind the rotation constant buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, NULL, &pCBuffer));
    pContext->VSSetConstantBuffers(0u, 1u, &pCBuffer);

    // Create the face color resource
    fc = {
        {
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f, 1.0f },
        }
    };
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = &fc;

    // Create the face color buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColors);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pCFaceColorBuffer));
    pContext->PSSetConstantBuffers(0, 1, &pCFaceColorBuffer);

    // select which primtive type we are using
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Map the constant buffer onto our mapped constant buffer
    ZeroMemory(&mCBuffer, sizeof(D3D11_MAPPED_SUBRESOURCE));
    pContext->Map(pCBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mCBuffer);
}

// this is the function used to render a single frame
void Graphics::RenderFrame(float angle, float xTranslation, float yTranslation)
{
    // clear the back buffer to a deep blue
    float color[4] = { 0.3f, 0.1f, 1.0f, 1.0f };
    pContext->ClearRenderTargetView(backbuffer, color);

    // Create the rotation constant
    float squeeze = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            dx::XMMatrixRotationX(2 * angle) *
            dx::XMMatrixRotationY(3 * angle) *
            dx::XMMatrixRotationZ(1 * angle) *
            dx::XMMatrixTranslation(xTranslation, yTranslation, 7.0f) *
            dx::XMMatrixPerspectiveLH(1.0f, squeeze, 0.5f, 10.0f)
        )
    };

    // Copy the data onto the map
    memcpy(mCBuffer.pData, &cb, sizeof(cb));

    // draw the vertex buffer to the back buffer
    pContext->DrawIndexed(indexCount, 0u, 0u);

    // switch the back buffer and the front buffer
    GFX_THROW_INFO(swapchain->Present(0u, 0u));
}

// this is the function that cleans up Direct3D and COM
void Graphics::CleanD3D()
{
    GFX_THROW_INFO(swapchain->SetFullscreenState(FALSE, NULL));

    // close and release all existing COM objects
    swapchain->Release();
    pDevice->Release();
    pContext->Release();
    backbuffer->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    pIBuffer->Release();
    pCBuffer->Release();

    // Unmap from the resources
    pContext->Unmap(pCBuffer, 0u);
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
