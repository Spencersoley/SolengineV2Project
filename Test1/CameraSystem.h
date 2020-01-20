#pragma once
#include <ShaderProgram.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include "TransformSystem.h"

struct CameraComponent
{
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

class CameraSystem
{
	TransformSystem* TM;
	SolengineV2::ShaderProgram* SP;
	glm::mat4 orthoMatrix;

public:
	CameraSystem(TransformSystem* tManager, SolengineV2::ShaderProgram* sp, int sh, int sw) :
		TM(tManager),
		SP(sp),
		orthoMatrix(1.0f),
		ScreenHeight(sh),
		ScreenWidth(sw),
		ActiveCam(nullptr),
		ActiveT(nullptr)
	{
		orthoMatrix = glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight);
	}
	std::map<int, CameraComponent> Cameras{};

	CameraComponent* ActiveCam;
	std::shared_ptr<TransformComponent> ActiveT;
	int ActiveCamID;
	int ScreenWidth, ScreenHeight;

	void AddComponent(int handle)
	{
		Cameras.insert(std::pair<int, CameraComponent>(handle, CameraComponent(TM->GetLast())));
		ActiveCam = &((--Cameras.end())->second);
		ActiveT = ActiveCam->Transform;
	}

	void Process()
	{
		if (ActiveT == nullptr || ActiveCam == nullptr) return;

		SP->Use();
		ActiveCam->NeedsMatrixUpdate |= ActiveT->IsUpdated; //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (ActiveCam->NeedsMatrixUpdate)//transform->Updated)
		{
			glm::vec3 translationVec(
				(ScreenWidth / 2) - ActiveT->Pos.x,
				(ScreenHeight / 2) - ActiveT->Pos.y,
				0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);

			glm::vec3 scaleVec(ActiveCam->Scale, ActiveCam->Scale, 1.0f);
			ActiveCam->ProjectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			ActiveCam->NeedsMatrixUpdate = false;
		}

		GLint pUniform = SP->GetUniformLocation("P");
		glm::mat4 projection = ActiveCam->ProjectionMatrix;
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(projection[0][0]));
	}

	void DeleteComponent(int handle)
	{
		Cameras.erase(handle);

		if (!Cameras.empty())
		{
			ActiveCam = &((--Cameras.end())->second);
			ActiveT = ActiveCam->Transform;
		}
		else
		{
			ActiveCam = nullptr;
			ActiveT = nullptr;
		}
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

	glm::vec2 ScreenToWorldConvert(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = ScreenHeight - screenCoords.y;
		//Converts 0 to centre
		screenCoords -= glm::vec2(ScreenWidth / 2, ScreenHeight / 2);
		//Scale the coordinates
		screenCoords /= ActiveCam->Scale;
		//Translate with camera;
		screenCoords += glm::vec2{ ActiveT->Pos.x, ActiveT->Pos.y };

		return screenCoords;
	}
};
