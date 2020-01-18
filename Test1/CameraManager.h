#pragma once
#include <ShaderProgram.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include "TransformManager.h"

struct CameraComponent
{
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

class CameraManager
{
	TransformManager* TM;
	SolengineV2::ShaderProgram* SP;
	glm::mat4 orthoMatrix;

public:
	CameraManager(TransformManager* tManager, SolengineV2::ShaderProgram* sp, int sh, int sw) :
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
	std::vector<CameraComponent> Cameras{};

	CameraComponent* ActiveCam;
	std::shared_ptr<TransformComponent> ActiveT;
	int ScreenWidth, ScreenHeight;

	void AddComponent()
	{
		Cameras.push_back(CameraComponent(TM->GetLastTransform()));
		ActiveCam = &Cameras.back();
		ActiveT = ActiveCam->transformCo;
	}

	void Process()
	{
		if (ActiveT == nullptr || ActiveCam == nullptr) return;

		if (TM->IsToBeDeleted(ActiveT.get()))
		{
			Cameras.pop_back();
			if (!Cameras.empty())
			{
				ActiveCam = &Cameras.back();
				ActiveT = ActiveCam->transformCo;
			}
			else
			{
				ActiveCam = nullptr;
				ActiveT = nullptr;
				return;
			}
		}

		SP->Use();
		ActiveCam->needsMatrixUpdate |= ActiveT->IsUpdated; //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (ActiveCam->needsMatrixUpdate)//transform->Updated)
		{
			glm::vec3 translationVec(
				(ScreenWidth / 2) - ActiveT->Pos.x,
				(ScreenHeight / 2) - ActiveT->Pos.y,
				0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);

			glm::vec3 scaleVec(ActiveCam->Scale, ActiveCam->Scale, 1.0f);
			ActiveCam->projectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			ActiveCam->needsMatrixUpdate = false;
		}

		GLint pUniform = SP->GetUniformLocation("P");
		glm::mat4 projection = ActiveCam->projectionMatrix;
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(projection[0][0]));
	}

	void Zoom(float zoom)
	{
		float newScale = ActiveCam->Scale + zoom;
		if (newScale < ActiveCam->MaxScale && newScale > ActiveCam->MinScale)
		{
			ActiveCam->Scale = newScale;
			ActiveCam->needsMatrixUpdate = true;
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
		screenCoords += glm::vec2 { ActiveT->Pos.x, ActiveT->Pos.y };

		return screenCoords;
	}
};
