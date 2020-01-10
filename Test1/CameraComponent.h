#pragma once

struct CameraComponent
{
	//CameraComponent() : CameraComponent(nullptr) {}
	CameraComponent(std::shared_ptr<TransformComponent> TC) : CameraComponent(TC, glm::mat4(0.0f), 1.0f, 5.0f, 0.5f) {}
	CameraComponent(std::shared_ptr<TransformComponent> TC, glm::mat4 projMat, float s, float maxS, float minS) :
		transformCo(TC),
		projectionMatrix(projMat),
		Scale(s), MaxScale(maxS),
		MinScale(minS),
		needsMatrixUpdate(true) {}

	glm::mat4 projectionMatrix;
	std::shared_ptr<TransformComponent> transformCo;
	float Scale;
	float MaxScale, MinScale;
	bool needsMatrixUpdate;
};
