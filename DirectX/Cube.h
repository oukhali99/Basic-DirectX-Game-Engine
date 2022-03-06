#include "Main.h"

class Cube {
public:
	Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    ~Cube();

    void RenderFrame(float angle, float xTranslation, float yTranslation, float xRot, float yRot, float zRot);
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
    ID3D11Device* pDevice;                      // the pointer to our Direct3D device interface
    ID3D11DeviceContext* pContext;              // the pointer to our Direct3D device context
    ID3D11Buffer* pVBuffer;
    ID3D11Buffer* pIBuffer;
    ID3D11Buffer* pCTransformationBuffer;
    ID3D11Buffer* pCFaceColorBuffer;
    D3D11_MAPPED_SUBRESOURCE mCBuffer;

	void InitGraphics();
    void HandleError(HRESULT hr, const char* file, const long long line);
};
