#pragma once
#include <SpriteBatch.h>
#include <memory>
#include "TransformSystem.h"
#include "CameraSystem.h"

struct GraphicsComponent
{
	GraphicsComponent(std::shared_ptr<TransformComponent> transform, GLuint textureID, SolengineV2::Colour colour) :
		Transform(transform),
		TextureID(textureID),
		Colour(colour),
		Redraw(true)
	{}

	std::shared_ptr<TransformComponent> Transform;
	SolengineV2::Colour Colour;
	GLuint TextureID;
	bool Redraw;
};

enum class GraphicsType { GENERAL, HEALTHBAR};

class GraphicsSystem
{
	TransformSystem* TS;
	CameraSystem* CS;

public:
	GraphicsSystem(TransformSystem* ts, CameraSystem* cs) : TS(ts), CS(cs) {}
	std::map<int, GraphicsComponent> Graphics{};
	std::map<int, GraphicsComponent> Healthbar{};

	SolengineV2::SpriteBatch SpriteBatch;

	void AddComponent(int handle, GraphicsType gt, GLuint64 textID, SolengineV2::Colour col)
	{
		if (gt == GraphicsType::GENERAL)
		{
			Graphics.insert(std::pair<int, GraphicsComponent>(handle, GraphicsComponent(TS->GetLast(), textID, col)));
		}
		else if (gt == GraphicsType::HEALTHBAR)
		{
			Healthbar.insert(std::pair<int, GraphicsComponent>(handle, GraphicsComponent(TS->GetLast(), textID, col)));
		}
	}

	void Process() //How do we solve draw order?
	{
		int sw = CS->ScreenWidth;
		int sh = CS->ScreenHeight;
		TransformComponent* CamTransform = CS->ActiveT.get();
		if (CamTransform == nullptr) return;
		float scale = CS->ActiveCam->Scale;

		SpriteBatch.Begin();

		renderComponents(Graphics, CamTransform, sw, sh, scale);
		renderComponents(Healthbar, CamTransform, sw, sh, scale);

		SpriteBatch.End();
		SpriteBatch.RenderSpriteBatch();
	}

	void DeleteComponent(int handle)
	{
		Graphics.erase(handle);
		Healthbar.erase(handle);
	}

private:
	void renderComponents(std::map<int, GraphicsComponent> map, TransformComponent* CamTransform, int sw, int sh, float scale)
	{
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			if (isInView(TC, CamTransform, sw, sh, scale))
			{
				const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
				glm::vec4 destRect
				{
					TC->Pos.x - TC->Dims.x / 2.0f,
					TC->Pos.y - TC->Dims.y / 2.0f,
					TC->Dims.x,
					TC->Dims.y
				};

				SpriteBatch.Draw(destRect, uvRect, it->second.TextureID, 0.0f, it->second.Colour);
			}
		}
	}

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