#pragma once
#include <memory>
#include <SpriteBatch.h>
#include <Font.h>
#include "TransformSystem.h"
#include "CameraSystem.h"

class SpriteComponent
{
private:
	friend class SpriteSystem;
	SpriteComponent(
		std::shared_ptr<TransformComponent> _transform,
		std::shared_ptr<SolengineV2::ShaderProgram> _shaderProgram,
		GLuint _textureID,
		SolengineV2::Colour _colour)
		:
		transform(_transform),
		shaderProgram(_shaderProgram),
		textureID(_textureID),
		colour(_colour),
		redraw(true),
		IsVisible(true)
	{}

	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram;
	SolengineV2::Colour colour;
	GLuint textureID;
	bool redraw;
	bool IsVisible;
};

enum class SpriteType : unsigned int { GENERAL = 0, WORLDUI = 1, UI = 2 };

class SpriteSystem
{
	TransformSystem* transformSystem;
	CameraSystem* cameraSystem;
	GLint selectionBoxTextureID;
	SolengineV2::SpriteBatch spriteBatch;
	SolengineV2::SpriteBatch uiBatch;

	std::map<SpriteType, std::map<int, SpriteComponent>> spriteMap{};

public:
	SpriteSystem(
		TransformSystem* _transformSystem,
		CameraSystem* _cameraSystem
	)
		:
		transformSystem(_transformSystem),
		cameraSystem(_cameraSystem),
		selectionBoxTextureID(0)
	{
		font1 = new SolengineV2::Font("Fonts/Hind/Hind-Medium.ttf", 32);
		sb = font1->GetSpriteBatch();
	}
	~SpriteSystem() 
	{
		delete font1;
		delete sb;
	}

	void AddComponent(int handle, std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram, SpriteType graphicsType, GLuint64 textID, SolengineV2::Colour col)
	{
		spriteMap[graphicsType].try_emplace(handle, SpriteComponent(transformSystem->GetLast(), shaderProgram, textID, col));
	}

	SolengineV2::Font* font1;
	SolengineV2::SpriteBatch* sb;

	void SetSelectionBoxTextureID(GLint sbtID)
	{
		selectionBoxTextureID = sbtID;
	}

	void Process() 
	{
		int sw = cameraSystem->GetScreenWidth();
		int sh = cameraSystem->GetScreenHeight();
		TransformComponent* CamTransform = cameraSystem->cameraTransform.get();
		if (CamTransform == nullptr) return;
		float scale = cameraSystem->GetActiveCamScale();

		spriteBatch.Begin();
		renderWorldComponents(spriteMap[SpriteType::GENERAL], CamTransform, sw, sh, scale);
		renderWorldComponents(spriteMap[SpriteType::WORLDUI], CamTransform, sw, sh, scale);
		spriteBatch.End();
		spriteBatch.RenderSpriteBatch();

		uiBatch.Begin();
		cameraSystem->ProcessUICamera();
		//renderUIComponents(UIGraphics, sw, sh);
		uiBatch.End();
		uiBatch.RenderSpriteBatch();

		
		cameraSystem->UnuseShader();
	}

	void SetColour(int handle, SolengineV2::Colour col = { 0, 150, 0, 155 }) 
	{
		auto it = spriteMap[SpriteType::GENERAL].find(handle);
		if (it != spriteMap[SpriteType::GENERAL].end())
		{
			it->second.colour = col;
		}
	}

	void SetVisibility(int handle, bool set)
	{
		auto it = spriteMap[SpriteType::WORLDUI].find(handle);
		if (it != spriteMap[SpriteType::WORLDUI].end())
		{
			it->second.IsVisible = set;
		}
	}

	void DeleteComponent(int handle) 
	{
		auto it = spriteMap.begin();
		while (it != spriteMap.end())
		{
			it->second.erase(handle);
			++it;
		}
	}

private:
	void renderUIComponents(std::map<int, SpriteComponent> uiGraphics, int sw, int sh) 
	{
		for (auto it = uiGraphics.begin(); it != uiGraphics.end(); ++it)
		{
			TransformComponent* TC = it->second.transform.get();
			glm::vec3 pos = transformSystem->GetPos(TC);
			glm::vec3 dims = transformSystem->GetDims(TC);
			const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
			glm::vec4 destRect
			{
				pos.x - dims.x / 2.0f,
				pos.y - dims.y / 2.0f,
				dims.x,
				dims.y
			};

			uiBatch.Draw(destRect, uvRect, it->second.textureID, 0.0f, it->second.colour);
		}
	}

	void renderWorldComponents(std::map<int, SpriteComponent> graphics, TransformComponent* CamTransform, int sw, int sh, float scale)
	{
		std::shared_ptr<SolengineV2::ShaderProgram> sp = nullptr;

		for (auto it = graphics.begin(); it != graphics.end(); ++it)
		{
			TransformComponent* TC = it->second.transform.get();
			if (isInView(TC, CamTransform, sw, sh, scale) && it->second.IsVisible)
			{
				if (it->second.shaderProgram != sp)
				{
					sp = it->second.shaderProgram;
					cameraSystem->ProcessWorldCamera(sp);
				}

				glm::vec3 pos = transformSystem->GetPos(TC);
				glm::vec3 dims = transformSystem->GetDims(TC);

				const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
				glm::vec4 destRect
				{
				pos.x - dims.x / 2.0f,
				pos.y - dims.y / 2.0f,
				dims.x,
				dims.y
				};

				// for drawing selection box
				if (transformSystem->GetIsSelected(TC))
				{
					glm::vec4 dr
					{
						pos.x - dims.x,
						pos.y - dims.y,
						dims.x * 2.0f,
						dims.y * 2.0f
					};

					spriteBatch.Draw(dr, uvRect, selectionBoxTextureID, 0.0f, SolengineV2::Colour{ 255, 255, 255, 255 });
				}

				spriteBatch.Draw(destRect, uvRect, it->second.textureID, 0.0f, it->second.colour);
			}
		}
	}

	//frustum culling
	bool isInView(TransformComponent* obj, TransformComponent* cam, int sw, int sh, float scale) const
	{
		glm::vec3 pos = transformSystem->GetPos(obj);
		glm::vec3 dims = transformSystem->GetDims(obj);

		glm::vec2 scaledScreenDims = glm::vec2(sw, sh) / scale;

		const float MIN_DISTANCE_X = dims.x / 2.0f + scaledScreenDims.x / 2.0f;
		const float MIN_DISTANCE_Y = dims.y / 2.0f + scaledScreenDims.y / 2.0f;

		//Get a vector of the distance between the colliding tile and the agent
		glm::vec2 centrePos = glm::vec2(pos.x, pos.y);
		glm::vec2 distVec = centrePos - glm::vec2(pos.x, pos.y);

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