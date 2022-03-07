#include "Main.h"
#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics& gfx, unsigned short indices[], UINT sizeOfIndices);
	void Bind(Transform transform) override;
};
