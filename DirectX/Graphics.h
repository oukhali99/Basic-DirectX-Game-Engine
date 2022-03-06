#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>

#include "Main.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define GFX_THROW_INFO(hrcall) if (FAILED(hr = (hrcall))) { HandleError(hr, __FILE__, __LINE__); }

namespace dx = DirectX;

class Graphics {
public:
    Graphics(HWND hWnd, unsigned short* indices, unsigned short indexCount);

    void RenderFrame(float angle, float xTranslation, float yTranslation);
    void CleanD3D();
private:
    struct VERTEX {
        float x, y, z;
    };

    struct ConstantBuffer {
        dx::XMMATRIX transformation;
    };

    struct FaceColors {
        struct {
            float r, g, b, a;
        } faceColors[12];
    };

    HRESULT hr;
    IDXGISwapChain* swapchain;                  // the pointer to the swap chain interface
    ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
    ID3D11DeviceContext* pContext;              // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;         // global declaration
    ID3D11VertexShader* pVS;                    // the vertex shader
    ID3D11PixelShader* pPS;                     // the pixel shader
    ID3D11Buffer* pVBuffer;                     // global
    ID3D11Buffer* pIBuffer;                     // global
    ID3D11Buffer* pCBuffer;
    ID3D11Buffer* pCFaceColorBuffer;
    ID3D11InputLayout* pLayout;                 // global
    D3D11_MAPPED_SUBRESOURCE mCBuffer;
    unsigned short* indices;
    unsigned short indexCount;
    FaceColors fc;

    void InitD3D(HWND hWnd);
    void InitPipeline();
    void InitGraphics();
    void HandleError(HRESULT hr, const char* file, const long long line);
};
