#include "Shape.h"

Shape::Shape(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform)
	:
	pDevice(pDevice),
	pContext(pContext),
	hr(0),
    transform(transform)
{}

Shape::~Shape() {
}

void Shape::HandleError(HRESULT hr, const char* file, const long long line) {
    std::stringstream ss;

    // Exception line
    ss << "Exception on line " << line << std::endl;

    // Exception file
    ss << "File: " << file << std::endl;

    // Error code
    ss << "Error code: " << "0x" << std::hex << hr << std::endl;

    // Error description
    char errorMessage[50];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorMessage,
        sizeof(errorMessage) / sizeof(errorMessage[0]),
        NULL
    );

    ss << "Description: " << errorMessage << std::endl;

    throw new std::exception(ss.str().c_str());
}
