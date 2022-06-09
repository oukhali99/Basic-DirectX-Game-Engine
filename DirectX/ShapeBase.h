#ifndef H_SHAPEBASE
#define H_SHAPEBASE
#include <vector>
#include "Shape.h"
#include "Bindable.h"

template<class T>
class ShapeBase : public Shape {
public:
	void Update() override {
        for (Bindable* bindable : bindables) {
            bindable->Bind(this);
        }
    }
protected:
    ShapeBase<T>(GameObject* gameObject, int vertexCount, int indexCount)
        :
        Shape(gameObject, vertexCount, indexCount)
    {}

	inline static std::vector<Bindable*> bindables;    
};
#endif
