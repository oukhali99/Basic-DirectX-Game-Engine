#include "Camera.h"
#include "bullet/btBulletDynamicsCommon.h"
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

	dx::XMVECTOR lookVector = dx::XMVectorSet(0, 0, 0.1f, 0);
	lookVector = dx::XMVector3Transform(
		lookVector,
		dx::XMMatrixRotationQuaternion(cameraRotation)
	);

	return dx::XMMatrixLookAtLH(cameraPosition, dx::XMVectorAdd(cameraPosition, lookVector), XMVectorSet(0, 1, 0, 0));
}

void Camera::Update() {}
