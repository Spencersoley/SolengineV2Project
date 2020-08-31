#include <glm\geometric.hpp>
#include "TransformSystemSpecialization.h"

void TransformSystem::translate(TransformComponent& component, const glm::vec2& transVec)  const
{
	Transform::System::setPos(component, Transform::System::getPos(component) + transVec);
}