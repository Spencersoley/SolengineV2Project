#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <ShaderManager.h>
#include "TransformSystem.h"

class CameraComponent
{
private:
	friend class CameraSystem;

	CameraComponent(std::shared_ptr<TransformComponent> _transform = nullptr, float _scale = 1.0f, float _maxScale = 5.0f, float _minScale = 0.5f)
		:
		transform(_transform),
		ProjectionMatrix(glm::mat4(0.0f)),
		scale(_scale),
		maxScale(_maxScale),
		minScale(_minScale),
		NeedsMatrixUpdate(true) 
	{}

	glm::mat4 ProjectionMatrix;
	std::shared_ptr<TransformComponent> transform;
	float scale, maxScale, minScale;
	bool NeedsMatrixUpdate;
};

class CameraSystem 
{
	std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram;
	SolengineV2::ShaderManager* shaderManager;
	glm::mat4 orthoMatrix;
	glm::mat4 uiMatrix;
	int screenWidth, screenHeight;

	TransformSystem* transformSystem;

public:
	CameraSystem(
		TransformSystem* _transformSystem, 
		std::shared_ptr<SolengineV2::ShaderProgram> _defaultShader, 
		SolengineV2::ShaderManager* _shaderProgramManager, 
		int _screenHeight = 500, 
		int _screenWidth = 500
	)
		:
		transformSystem(_transformSystem),
		shaderProgram(_defaultShader),
		shaderManager(_shaderProgramManager),
		orthoMatrix(1.0f),
		screenHeight(_screenHeight),
		screenWidth(_screenWidth),
		cameraComponent(nullptr),
		cameraTransform(nullptr)
	{
		orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
		uiMatrix = glm::translate(orthoMatrix, glm::vec3{ (screenWidth / 2), (screenHeight / 2), 0.0f });
	}

	std::shared_ptr<CameraComponent> cameraComponent;
	std::shared_ptr<TransformComponent> cameraTransform;
	
	int GetScreenHeight() { return screenHeight; }
	int GetScreenWidth() { return screenWidth; }

	//Sets the last cameracomponent added as the desired camera. 
	//Designed for single use
	void SetCamera()
	{
		std::shared_ptr<CameraComponent> cam(new CameraComponent(transformSystem->GetLast()));
		cameraComponent = cam;
		cameraTransform = cameraComponent->transform;
	}

	//processing camera involves updating the projection matrix, updating the projection matrix in the shader used for rendering
	void ProcessWorldCamera(const std::shared_ptr<SolengineV2::ShaderProgram> sp) const
	{
		if (cameraTransform == nullptr || cameraComponent == nullptr) return;
		
		cameraComponent->NeedsMatrixUpdate |= transformSystem->GetIsUpdated(cameraTransform.get()); //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (cameraComponent->NeedsMatrixUpdate)
		{
			glm::vec3 translationVec(
				(screenWidth / 2) - transformSystem->GetPos(cameraTransform.get()).x, 
				(screenHeight / 2) - transformSystem->GetPos(cameraTransform.get()).y, 
				0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);
			glm::vec3 scaleVec(cameraComponent->scale, cameraComponent->scale, 1.0f);
			cameraComponent->ProjectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			cameraComponent->NeedsMatrixUpdate = false;
		}

		shaderManager->SetProjectionMatrix(sp, cameraComponent->ProjectionMatrix);
	}

	void ProcessUICamera() const
	{	
		shaderManager->SetProjectionMatrix(shaderProgram, uiMatrix);
	}

	void UnuseShader() const
	{
		shaderManager->Unuse(shaderProgram);
	}

	void Zoom(float zoom) const
	{
		float newScale = cameraComponent->scale + zoom;
		if (newScale < cameraComponent->maxScale && newScale > cameraComponent->minScale)
		{
			cameraComponent->scale = newScale;
			cameraComponent->NeedsMatrixUpdate = true;
		}
	}

	float GetActiveCamScale() const
	{
		return cameraComponent->scale;
	}

	glm::vec2 ScreenToWorldConvert(glm::vec2 screenCoords) const
	{
		//Invert Y direction
		screenCoords.y = screenHeight - screenCoords.y;
		//Converts 0 to centre
		screenCoords -= glm::vec2{ screenWidth / 2, screenHeight / 2 };
		//Scale the coordinates
		screenCoords /= cameraComponent->scale;
		//Translate with camera;
		screenCoords += glm::vec2{ transformSystem->GetPos(cameraTransform.get()).x, transformSystem->GetPos(cameraTransform.get()).y };

		return screenCoords;
	}
};
