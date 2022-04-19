#include "PositionConstraint.h"

PositionConstraint::PositionConstraint(GameObject* gameObject)
	:
	Component(gameObject),
	constrainer(0),
	distance()
{}

void PositionConstraint::Update() {
	if (constrainer) {
		btTransform newTransform = gameObject->GetTransform();
		newTransform.setOrigin(constrainer->GetTransform().getOrigin() + distance);
		gameObject->SetTransform(newTransform);
	}
}

void PositionConstraint::SetConstrainer(GameObject* constrainer) {
	btVector3 distance = gameObject->GetTransform().getOrigin() - constrainer->GetTransform().getOrigin();
	this->constrainer = constrainer;
}
