#include "Main.h"
#include "Shape.h"
#include "Clock.h"
#include "Mouse.h"
#include "VertexBuffer.h"
#include "TransformConstantBuffer.h"

class Cube : public Shape {
public:
	Cube(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);
    ~Cube();

    bool followMouse;

    void RenderFrame() override;
    void OnButtonPressed(WPARAM wParam) override;
    void OnMouseMovedTo(Mouse::Position position) override;
private:
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
