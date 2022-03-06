#include "Main.h"
#include "Cube.h"

class Graphics {
public:
    Graphics(HWND hWnd, unsigned short* indices, unsigned short indexCount);
    ~Graphics();

    void RenderFrame(float angle, float xTranslation, float yTranslation);
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
    ID3D11InputLayout* pLayout;                 // global
    unsigned short* indices;
    unsigned short indexCount;
    Cube* cube1;
    Cube* cube2;

    void InitD3D(HWND hWnd);
    void InitPipeline();
    void HandleError(HRESULT hr, const char* file, const long long line);
};
