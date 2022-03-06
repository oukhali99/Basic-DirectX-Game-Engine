#include "Main.h"
#include "Shape.h"
#include "Clock.h"

class Cube : public Shape {
public:
	Cube(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);
    ~Cube();

    void RenderFrame() override;
    void ButtonPressed(WPARAM wParam) override;
private:
    struct ConstantBuffer {
        dx::XMMATRIX transformation;
    };

    struct FaceColors {
        struct {
            float r, g, b, a;
        } faceColors[6];
    };

    ID3D11Buffer* pVBuffer;
    ID3D11Buffer* pIBuffer;
    ID3D11Buffer* pCTransformationBuffer;
    ID3D11Buffer* pCFaceColorBuffer;
    D3D11_MAPPED_SUBRESOURCE mCBuffer;

	void InitGraphics();
};
