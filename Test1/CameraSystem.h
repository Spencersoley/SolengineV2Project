#pragma once
#include <ShaderProgram.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include "TransformSystem.h"
#include "System.h"

class CameraComponent
{
private:
	friend class CameraSystem;
	CameraComponent(std::shared_ptr<TransformComponent> transform) : CameraComponent(transform, glm::mat4(0.0f), 1.0f, 5.0f, 0.5f) {}
	CameraComponent(std::shared_ptr<TransformComponent> transform, glm::mat4 projectionMatrix, float scale, float maxScale, float minScale) :
		Transform(transform),
		ProjectionMatrix(projectionMatrix),
		Scale(scale), 
		MaxScale(maxScale),
		MinScale(minScale),
		NeedsMatrixUpdate(true) {}

	glm::mat4 ProjectionMatrix;
	std::shared_ptr<TransformComponent> Transform;
	float Scale;
	float MaxScale, MinScale;
	bool NeedsMatrixUpdate;
};

class CameraSystem : public System
{
	TransformSystem* TS;
	SolengineV2::ShaderProgram* SP;
	SolengineV2::ShaderProgramManager* SPM;
	glm::mat4 orthoMatrix;
	glm::mat4 uiMatrix;

public:
	CameraSystem(TransformSystem* tManager, SolengineV2::ShaderProgram* sp, SolengineV2::ShaderProgramManager* spm, int sh, int sw) :
		TS(tManager),
		SP(sp),
		SPM(spm),
		orthoMatrix(1.0f),
		ScreenHeight(sh),
		ScreenWidth(sw),
		ActiveCam(nullptr),
		ActiveT(nullptr)
	{
		orthoMatrix = glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight);
		uiMatrix = glm::translate(orthoMatrix, glm::vec3{ (ScreenWidth / 2), (ScreenHeight / 2), 0.0f });
	}
	std::map<int, CameraComponent> WorldCameras{};

	CameraComponent* ActiveCam;
	std::shared_ptr<TransformComponent> ActiveT;
	int ScreenWidth, ScreenHeight;

	void AddComponent(int handle)
	{
		WorldCameras.insert(std::pair<int, CameraComponent>(handle, CameraComponent(TS->GetLast())));
		ActiveCam = &((--WorldCameras.end())->second);
		ActiveT = ActiveCam->Transform;
	}

	void ProcessWorldCamera(const SolengineV2::ShaderProgram* sp)
	{
		if (ActiveT == nullptr || ActiveCam == nullptr) return;
		
		ActiveCam->NeedsMatrixUpdate |= TS->GetIsUpdated(ActiveT.get()); //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (ActiveCam->NeedsMatrixUpdate)
		{
			glm::vec3 translationVec((ScreenWidth / 2) - TS->GetPos(ActiveT.get()).x, (ScreenHeight / 2) - TS->GetPos(ActiveT.get()).y, 0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);
			glm::vec3 scaleVec(ActiveCam->Scale, ActiveCam->Scale, 1.0f);
			ActiveCam->ProjectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			ActiveCam->NeedsMatrixUpdate = false;
		}

		SPM->SetProjectionMatrix(sp, ActiveCam->ProjectionMatrix);
	}

	void ProcessUICamera()
	{	
		SPM->SetProjectionMatrix(SP, uiMatrix);
	}

	void UnuseShader()
	{
		SPM->Unuse(SP);
	}

	void DeleteComponent(int handle)
	{
		WorldCameras.erase(handle);
		if (!NewCamera()) throw std::invalid_argument("No camera found! you must have a camera to render world content");
	}

	void Zoom(float zoom)
	{
		float newScale = ActiveCam->Scale + zoom;
		if (newScale < ActiveCam->MaxScale && newScale > ActiveCam->MinScale)
		{
			ActiveCam->Scale = newScale;
			ActiveCam->NeedsMatrixUpdate = true;
		}
	}

	float GetActiveCamScale()
	{
		return ActiveCam->Scale;
	}

	glm::vec2 ScreenToWorldConvert(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = ScreenHeight - screenCoords.y;
		//Converts 0 to centre
		screenCoords -= glm::vec2{ ScreenWidth / 2, ScreenHeight / 2 };
		//Scale the coordinates
		screenCoords /= ActiveCam->Scale;
		//Translate with camera;
		screenCoords += glm::vec2{ TS->GetPos(ActiveT.get()).x, TS->GetPos(ActiveT.get()).y };

		return screenCoords;
	}

private:

	bool NewCamera()
	{
		if (!WorldCameras.empty())
		{
			ActiveCam = &((--WorldCameras.end())->second);
			ActiveT = ActiveCam->Transform;
			return true;
		}
		else
		{
			return false;
		}
	}
};
