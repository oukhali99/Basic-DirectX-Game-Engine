#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <map>
#include <string>
#include "Main.h"
#include "Shape.h"
#include "Bindable.h"
#include "Light.h"

#define SHADER_FILE_NAME_DEFAULT L"DefaultShaders.hlsl"
#define SHADER_FILE_NAME_TEXTURE L"TextureShaders.hlsl"
#define SHADER_FILE_NAME_SHADOW_MAP L"ShadowMapShaders.hlsl"

class Gui;

struct VERTEX {
    float position[3];
    float normal[3];
    float texCoords[2];
};

struct FaceColor {
    float r, g, b, a;
};

class Graphics {
public:
    static void Init(HWND hWnd, float nearZ, float farZ);
    static Graphics* GetInstance();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    float GetNearZ();
    float GetFarZ();

    void ClearFrame();
    void RenderFrame();
    void SetShaders(LPCWSTR shaderFileName);
    void SetNearZ(float nearZ);
    void SetFarZ(float farZ);
    void BindLightingBuffer();
    void AddLight(Light* light);
private:
    Graphics(HWND hWnd, float nearZ, float farZ);
    ~Graphics();
    inline static Graphics* instance;

    HRESULT hr;
    HWND hWnd;
    IDXGISwapChain* swapchain;                  // the pointer to the swap chain interface
    ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
    ID3D11DeviceContext* pContext;              // the pointer to our Direct3D device context
    ID3D11RenderTargetView* renderTargetView;   // global declaration
    ID3D11InputLayout* pLayout;                 // global
    ID3D11DepthStencilView* pDSView;
    float nearZ, farZ;
    std::map<LPCWSTR, std::pair<ID3D11VertexShader*, ID3D11PixelShader*>> compiledShaders;
    ID3D11Buffer* lightingBuffer;
    std::vector<Light::LightData*> lightDataVector;

    void InitD3D();
    void InitPipeline();
    void InitGraphics();
    void InitLightingBuffer();
};
#endif
