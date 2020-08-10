#pragma once
#include "CameraComponent.h"

template <class Implementation>
class CameraSystemInterface
{
public:
	static float getScale(const CameraComponent& component) { return component.scale; } 
	static const glm::mat4& getProjectionMatrix(const CameraComponent& component)  { return component.projectionMatrix; }
	static const bool getHasMoved(const CameraComponent& component) { return component.hasMoved; }

	static void setScale(CameraComponent& component, const float set) { component.scale = set; }
	static void setProjectionMatrix(CameraComponent& component, const glm::mat4& set) { component.projectionMatrix = set; }
	static void setHasMoved(CameraComponent& component, const bool set) { component.hasMoved = set; }

	static void updateCameraMatrices(CameraComponent& component, const TransformComponent& cameraTransform)  
	{
		static Implementation system;
		system.updateCameraMatrices(component, cameraTransform);
	}

	static void zoom(CameraComponent& component, float zoom)
	{
		static Implementation system;
		system.zoom(component, zoom);
	}

	static glm::vec2 convertScreenToWorld(const CameraComponent& cameraComponent, const TransformComponent& transformComponent, const glm::vec2 screenCoords)
	{
		static Implementation system;
		return system.convertScreenToWorld(cameraComponent, transformComponent, screenCoords);
	}
};

