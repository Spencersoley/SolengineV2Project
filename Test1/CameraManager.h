#pragma once
#include <ShaderProgram.h>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include "TransformManager.h"
#include "CameraComponent.h"

class CameraManager
{
public:
	CameraManager() : CameraManager(nullptr, nullptr, 0, 0) {}
	CameraManager(TransformManager* tManager, SolengineV2::ShaderProgram* sp, int sh, int sw) :
		TM(tManager),
		activeShaderProgram(sp),
		orthoMatrix(1.0f),
		ScreenHeight(sh),
		ScreenWidth(sw),
		activeC(nullptr),
		activeT(nullptr)
	{
		//camDims = { screenWidth / scale, screenHeight / scale };
		orthoMatrix = glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight);
	}

	std::vector<CameraComponent> Cameras{};

	TransformManager* TM;
	int ScreenWidth, ScreenHeight;
	CameraComponent* activeC;
	std::shared_ptr<TransformComponent> activeT;


	void Zoom(float zoom)
	{
		float newScale = activeC->Scale + zoom;
		if (newScale < activeC->MaxScale && newScale > activeC->MinScale)
		{
			activeC->Scale = newScale;
			activeC->needsMatrixUpdate = true;
		}
	}

	void Process()
	{
		if (activeT == nullptr || activeC == nullptr) return;

		if (TM->IsToBeDeleted(activeT.get()))
		{
			Cameras.pop_back();
			if (!Cameras.empty())
			{
				activeC = &Cameras.back();
				activeT = activeC->transformCo;
			}
			else
			{
				activeC = nullptr;
				activeT = nullptr;
				return;
			}
		}

		activeShaderProgram->Use();
		activeC->needsMatrixUpdate |= activeT->IsUpdated; //PROCESS CAMERA BEFORE GRAPHICS, AFTER PHYSICS

		if (activeC->needsMatrixUpdate)//transform->Updated)
		{
			glm::vec3 translationVec(
				(ScreenWidth / 2) - activeT->Pos.x,
				(ScreenHeight / 2) - activeT->Pos.y,
				0.0f);
			glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);

			glm::vec3 scaleVec(activeC->Scale, activeC->Scale, 1.0f);
			activeC->projectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
			activeC->needsMatrixUpdate = false;
		}

		GLint pUniform = activeShaderProgram->GetUniformLocation("P");
		glm::mat4 projection = activeC->projectionMatrix;
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(projection[0][0]));
	}

	void AddComponent()
	{
		Cameras.push_back(CameraComponent(TM->GetLastTransform()));
		activeC = &Cameras.back();
		activeT = activeC->transformCo;
	}

private:
	SolengineV2::ShaderProgram* activeShaderProgram;
	glm::mat4 orthoMatrix;
};
