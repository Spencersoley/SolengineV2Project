#pragma once
#include <SpriteBatch.h>
#include <memory>
#include "TransformManager.h"
#include "CameraManager.h"
#include "GraphicsComponent.h"

class GraphicsManager
{
	TransformManager* TM;
	CameraManager* CM;

public:
	GraphicsManager(TransformManager* TManager, CameraManager* CManager) : TM(TManager), CM(CManager) { }

	std::vector<GraphicsComponent> Graphics{};
	std::vector<SolengineV2::SpriteBatch*> SpriteBatches{ new SolengineV2::SpriteBatch() };

	void Init()
	{
		std::sort(Graphics.begin(), Graphics.end(), [](const GraphicsComponent& a, const GraphicsComponent& b)
			{ return a.transformCo->Pos.z < b.transformCo->Pos.z; });
	}

	void Process()
	{
		int sw = CM->ScreenWidth;
		int sh = CM->ScreenHeight;
		TransformComponent* CamTransform = CM->activeT.get();
		if (CamTransform == nullptr) return;
		float scale = CM->activeC->Scale;

		SpriteBatches[0]->Begin();

		for (auto it = Graphics.begin(); it != Graphics.end();)
		{
			TransformComponent* TC = it->transformCo.get();
			if (TM->IsToBeDeleted(TC))
			{
				std::cout << "GIM-----------USE COUNT: " << it->transformCo.use_count() << std::endl;
				it = Graphics.erase(it);
				std::cout << "Graphics size: " << Graphics.size() << std::endl;
			}
			else if (isInView(TC, CamTransform, sw, sh, scale))
			{
				

				const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
				glm::vec4 destRect
				{
					TC->Pos.x - TC->Dims.x / 2.0f,
					TC->Pos.y - TC->Dims.y / 2.0f,
					TC->Dims.x,
					TC->Dims.x
				};

				SpriteBatches[0]->Draw(destRect, uvRect, it->textureID, 0.0f, it->colour);
				++it;
			}
			else
			{
				++it;
			}
		}

		SpriteBatches[0]->End();
		SpriteBatches[0]->RenderSpriteBatch();
	}

	void AddComponent(GLuint64 textID, SolengineV2::Colour col)
	{
		Graphics.push_back(GraphicsComponent(SpriteBatches[0], TM->GetLastTransform(), textID, col));
	}

private:
	bool isInView(TransformComponent* obj, TransformComponent* cam, int sw, int sh, float scale)
	{
		glm::vec2 scaledScreenDims = glm::vec2(sw, sh) / scale;

		const float MIN_DISTANCE_X = obj->Dims.x / 2.0f + scaledScreenDims.x / 2.0f;
		const float MIN_DISTANCE_Y = obj->Dims.y / 2.0f + scaledScreenDims.y / 2.0f;

		//Get a vector of the distance between the colliding tile and the agent
		glm::vec2 centrePos = glm::vec2(obj->Pos.x, obj->Pos.y);
		glm::vec2 distVec = centrePos - glm::vec2(cam->Pos.x, cam->Pos.y);

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		// If the minimum collision distance is greater than the current distance, we have a collision
		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
};