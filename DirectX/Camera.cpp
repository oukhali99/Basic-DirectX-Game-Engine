#include "Camera.h"
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

Camera::Camera(GameObject* gameObject)
	:
	Component(gameObject)
{}

dx::XMMATRIX Camera::GetMatrix() {
	btTransform transform = gameObject->GetTransform();
	dx::XMVECTOR cameraPosition = dx::XMVectorSet(
		transform.getOrigin().x(),
		transform.getOrigin().y(),
		transform.getOrigin().z(),
		transform.getOrigin().w()
	);
	dx::XMVECTOR cameraRotation = dx::XMVectorSet(
		transform.getRotation().x(),
		transform.getRotation().y(),
		transform.getRotation().z(),
		transform.getRotation().w()
	);

	dx::XMVECTOR lookVector = dx::XMVectorSet(0, 0, 1, 0);
	lookVector = dx::XMVector3Transform(
		lookVector,
		dx::XMMatrixTranslationFromVector(cameraPosition) *
		dx::XMMatrixRotationQuaternion(cameraRotation)
	);

	return dx::XMMatrixLookAtLH(cameraPosition, cameraPosition + lookVector, XMVectorSet(0, 1, 0, 0));
}

void Camera::Update() {}
