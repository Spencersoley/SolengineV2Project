#include <glm\geometric.hpp>
#include "TransformSystemSpecialization.h"
#include "TransformSystemBounded.h"

void TransformSystemBounded::translate(TransformComponent& component, const glm::vec2& transVec, float arenaDiameter) const
{
	glm::vec2 newPos = Transform::System::getPos(component) + transVec;
	float distanceOutOfBounds = glm::distance(newPos, { 0, 0 }) - (arenaDiameter / 2.0f);
	if (distanceOutOfBounds > 0)
	{
		glm::vec2 normalizedCorrectionVec = glm::normalize(-newPos);
		Transform::System::setPos(component, newPos + normalizedCorrectionVec * distanceOutOfBounds);
	}
	else
	{
		Transform::System::setPos(component, newPos);
	}
}