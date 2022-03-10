#ifndef H_GRAPHICS
#define H_GRAPHICS
#include "Main.h"
#include "Shape.h"
#include "Bindable.h"

class Graphics {
    friend class Bindable;
    friend class Shape;
public:
    Graphics(HWND hWnd, float nearZ, float farZ);
    ~Graphics();

    ID3D11Device* GetPDevice();
    ID3D11DeviceContext* GetPContext();

    // Mutators
    void AddShape(Shape* shape);

    void RenderFrame();
    void ButtonPressed(WPARAM wParam);
private:
    HRESULT hr;
    HWND hWnd;
    IDXGISwapChain* swapchain;                  // the pointer to the swap chain interface
    ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
    ID3D11DeviceContext* pContext;              // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;         // global declaration
    ID3D11VertexShader* pVS;                    // the vertex shader
    ID3D11PixelShader* pPS;                     // the pixel shader
    ID3D11InputLayout* pLayout;                 // global
    ID3D11DepthStencilView* pDSView;
    float nearZ, farZ;

    std::vector<Shape*> shapes;

    void InitD3D();
    void InitPipeline();
    void InitGraphics();
    void HandleError(HRESULT hr, const char* file, const long long line);
};
#endif
