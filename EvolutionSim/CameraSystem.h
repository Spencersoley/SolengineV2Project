#pragma once
#include <ShaderProgram.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
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
	SolengineV2::ShaderProgram* shaderProgram;
	SolengineV2::ShaderProgramManager* shaderProgramManager;
	glm::mat4 orthoMatrix;
	glm::mat4 uiMatrix;
	int screenWidth, screenHeight;

	TransformSystem* transformSystem;

	std::map<int, CameraComponent> worldCameras{};

public:
	CameraSystem(TransformSystem* _transformSystem, 
		SolengineV2::ShaderProgram* _shaderProgram, 
		SolengineV2::ShaderProgramManager* _shaderProgramManager, 
		int _screenHeight = 500, 
		int _screenWidth = 500)
		:
		transformSystem(_transformSystem),
		shaderProgram(_shaderProgram),
		shaderProgramManager(_shaderProgramManager),
		orthoMatrix(1.0f),
		screenHeight(_screenHeight),
		screenWidth(_screenWidth),
		ActiveCam(nullptr),
		ActiveT(nullptr)
	{
		orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
		uiMatrix = glm::translate(orthoMatrix, glm::vec3{ (screenWidth / 2), (screenHeight / 2), 0.0f });
	}

	CameraComponent* ActiveCam;
	std::shared_ptr<TransformComponent> ActiveT;
	
	int GetScreenHeight() { return screenHeight; }
	int GetScreenWidth() { return screenWidth; }

	void AddComponent(int handle)
	{
		//worldCameras[handle] = CameraComponent(transformSystem->GetLast()); requires private access that it doesn't have
		worldCameras.try_emplace(handle, CameraComponent(transformSystem->GetLast())); //insert won't replace if it already exists at handle
		ActiveCam = &((--worldCameras.end())->second);
		ActiveT = ActiveCam->transform;
	}

	void ProcessWorldCamera(const SolengineV2::ShaderProgram* sp)
	{
		if (ActiveT == nullptr || ActiveCam == nullptr) return;
		
		ActiveCam->NeedsMatrixUpdate |= transformSystem->GetIsUpdated(ActiveT.get()); //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (ActiveCam->NeedsMatrixUpdate)
		{
			glm::vec3 translationVec((screenWidth / 2) - transformSystem->GetPos(ActiveT.get()).x, (screenHeight / 2) - transformSystem->GetPos(ActiveT.get()).y, 0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);
			glm::vec3 scaleVec(ActiveCam->scale, ActiveCam->scale, 1.0f);
			ActiveCam->ProjectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			ActiveCam->NeedsMatrixUpdate = false;
		}

		shaderProgramManager->SetProjectionMatrix(sp, ActiveCam->ProjectionMatrix);
	}

	void ProcessUICamera()
	{	
		shaderProgramManager->SetProjectionMatrix(shaderProgram, uiMatrix);
	}

	void UnuseShader()
	{
		shaderProgramManager->Unuse(shaderProgram);
	}

	void DeleteComponent(int handle)
	{
		worldCameras.erase(handle);
		if (!NewCamera()) throw std::invalid_argument("No camera found! you must have a camera to render world content");
	}

	void Zoom(float zoom)
	{
		float newScale = ActiveCam->scale + zoom;
		if (newScale < ActiveCam->maxScale && newScale > ActiveCam->minScale)
		{
			ActiveCam->scale = newScale;
			ActiveCam->NeedsMatrixUpdate = true;
		}
	}

	float GetActiveCamScale()
	{
		return ActiveCam->scale;
	}

	glm::vec2 ScreenToWorldConvert(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = screenHeight - screenCoords.y;
		//Converts 0 to centre
		screenCoords -= glm::vec2{ screenWidth / 2, screenHeight / 2 };
		//Scale the coordinates
		screenCoords /= ActiveCam->scale;
		//Translate with camera;
		screenCoords += glm::vec2{ transformSystem->GetPos(ActiveT.get()).x, transformSystem->GetPos(ActiveT.get()).y };

		return screenCoords;
	}

private:

	bool NewCamera()
	{
		if (!worldCameras.empty())
		{
			ActiveCam = &((--worldCameras.end())->second);
			ActiveT = ActiveCam->transform;
			return true;
		}
		else
		{
			return false;
		}
	}
};
