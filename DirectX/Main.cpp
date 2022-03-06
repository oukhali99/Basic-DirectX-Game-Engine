#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <comdef.h>
#include <atlstr.h>
#include <string>
#include <DirectXMath.h>
#include <chrono>
#include <sstream>

namespace dx = DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// Macros
void HandleError(HRESULT hr, const char* file, const long long line) {
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
HRESULT hr;
#define GFX_THROW_INFO(hrcall) if (FAILED(hr = (hrcall))) { HandleError(hr, __FILE__, __LINE__); }

struct VERTEX {
    float x, y, z;              // position
};

struct ConstantBuffer {
    dx::XMMATRIX transformation;
};

struct FaceColors {
    struct {
        float r, g, b, a;
    } faceColors[6];
};

// global declarations
IDXGISwapChain* swapchain;              // the pointer to the swap chain interface
ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
ID3D11DeviceContext* pContext;            // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;     // global declaration
ID3D11VertexShader* pVS;                // the vertex shader
ID3D11PixelShader* pPS;                 // the pixel shader
ID3D11Buffer* pVBuffer;                 // global
ID3D11Buffer* pIBuffer;                 // global
ID3D11Buffer* pCBuffer;
ID3D11Buffer* pCFaceColorBuffer;
ID3D11InputLayout* pLayout;             // global
D3D11_MAPPED_SUBRESOURCE mCBuffer;
float xTranslation, yTranslation;

const unsigned short indices[] = {
    0, 1, 2, 3, 0, 2,
    4, 6, 5, 6, 4, 7,
    0, 4, 1, 1, 4, 5,
    3, 6, 7, 3, 2, 6,
    3, 7, 0, 0, 7, 4,
    2, 5, 6, 1, 5, 2
};

// function prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory
void RenderFrame(float angle);
void InitPipeline();
void InitGraphics();

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    try {
        HWND hWnd;

        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        //RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };    // set the size, but not the position
        //AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(
            NULL,
            L"WindowClass1",    // name of the window class
            L"Our First Windowed Program",   // title of the window
            WS_OVERLAPPEDWINDOW,    // window style
            300,    // x-position of the window
            100,    // y-position of the window
            SCREEN_WIDTH,    // width of the window
            SCREEN_HEIGHT,    // height of the window
            NULL,    // we have no parent window, NULL
            NULL,    // we aren't using menus, NULL
            hInstance,    // application handle
            NULL);    // used with multiple windows, NULL

        if (hWnd == nullptr) {
            throw new std::exception("hWnd was found to be null");
        }

        // display the window on the screen
        ShowWindow(hWnd, nCmdShow);

        // set up and initialize Direct3D
        InitD3D(hWnd);

        // this struct holds Windows event messages
        MSG msg = { 0 };

        // wait for the next message in the queue, store the result in 'msg'
        auto startTime = std::chrono::steady_clock::now();
        while (true)
        {
            if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
            }
            else {
                const float t = std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();
                std::ostringstream oss;
                oss << "Time elapsed: " << std::fixed << t << "s";
                SetWindowTextA(hWnd, oss.str().c_str());

                RenderFrame(t);

                // switch the back buffer and the front buffer
                GFX_THROW_INFO(swapchain->Present(0u, 0u));
            }
        }

        CleanD3D();

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

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            // close the application entirely
            PostQuitMessage(0);
            break;
        case WM_CHAR:
            if (wParam == 'w') {
                yTranslation += 1.0f / 5.0f;
            }
            else if (wParam == 's') {
                yTranslation -= 1.0f / 5.0f;
            }
            else if (wParam == 'a') {
                xTranslation -= 1.0f / 5.0f;
            }
            else if (wParam == 'd') {
                xTranslation += 1.0f / 5.0f;
            }
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
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

    InitPipeline();
    InitGraphics();
}

// this is the function that cleans up Direct3D and COM
void CleanD3D()
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

void InitPipeline()
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

void InitGraphics() {
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
    bd.Usage = D3D11_USAGE_DYNAMIC;                 // write access access by CPU and GPU
    bd.ByteWidth = sizeof(indices);                 // size is the VERTEX struct * 3
    bd.StructureByteStride = sizeof(unsigned short);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;         // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // allow CPU to write in buffer
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
    FaceColors fc = {
        {
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
void RenderFrame(float angle)
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
    pContext->DrawIndexed(sizeof(indices), 0u, 0u);
}
