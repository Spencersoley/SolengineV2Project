#pragma once

struct TransformComponent
{
	TransformComponent() : TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f)) {}
	TransformComponent(glm::vec3 pos, glm::vec3 dims) :
		Pos(pos),
		Dims(dims),
		IsUpdated(true),
		deletable(false),
		toBeDeleted(false) {}

	glm::vec3 Pos;
	glm::vec3 Dims;
	bool IsUpdated;
private:
	friend class TransformManager;
	bool deletable;
	bool toBeDeleted;
};
