#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <map>
#include <string>
#include "Main.h"
#include "Shape.h"
#include "Bindable.h"

#define SHADER_FILE_NAME_DEFAULT L"DefaultShaders.hlsl"
#define SHADER_FILE_NAME_TEXTURE L"TextureShaders.hlsl"

struct VERTEX {
    float position[3];
    float texCoords[2];
};

struct ConstantBuffer {
    dx::XMMATRIX transformation;
};

struct FaceColor {
    float r, g, b, a;
};

class Graphics {
    friend class Bindable;
    friend class Shape;
public:
    static void Init(HWND hWnd, float nearZ, float farZ);
    static Graphics* GetInstance();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    void ClearFrame();
    void RenderFrame();
    void SetShaders(LPCWSTR shaderFileName);
private:
    Graphics(HWND hWnd, float nearZ, float farZ);
    ~Graphics();
    inline static Graphics* instance;

    HRESULT hr;
    HWND hWnd;
    IDXGISwapChain* swapchain;                  // the pointer to the swap chain interface
    ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
    ID3D11DeviceContext* pContext;              // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;         // global declaration
    ID3D11InputLayout* pLayout;                 // global
    ID3D11DepthStencilView* pDSView;
    float nearZ, farZ;
    std::map<LPCWSTR, std::pair<ID3D11VertexShader*, ID3D11PixelShader*>> compiledShaders;

    void InitD3D();
    void InitPipeline();
    void InitGraphics();
};
#endif
