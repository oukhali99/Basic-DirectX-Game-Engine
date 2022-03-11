#ifndef H_GRAPHICS
#define H_GRAPHICS
#include "Main.h"
#include "Shape.h"
#include "Bindable.h"

struct VERTEX {
    float position[3];
    float texCoords[2];
};

struct ConstantBuffer {
    dx::XMMATRIX transformation;
};

struct FaceColors {
    struct {
        float r, g, b, a;
    } faceColors[6];
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

    void InitD3D();
    void InitPipeline();
    void InitGraphics();
};
#endif
