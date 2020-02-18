#pragma once
#include <SpriteBatch.h>
#include <memory>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include <Font.h>

class GraphicsComponent
{
private:
	friend class GraphicsSystem;
	GraphicsComponent(
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

enum class GraphicsType : unsigned int { GENERAL = 0, WORLDUI = 1, UI = 2 };

class GraphicsSystem
{
	TransformSystem* TS;
	CameraSystem* CS;
	GLint selectionBoxTextureID;
	SolengineV2::SpriteBatch SpriteBatch;
	SolengineV2::SpriteBatch UIBatch;

	std::map<int, GraphicsComponent> WorldGraphics{};
	std::map<int, GraphicsComponent> WorldUIGraphics{};
	std::map<int, GraphicsComponent> UIGraphics{};

public:
	GraphicsSystem(TransformSystem* ts, CameraSystem* cs)
		:
		TS(ts),
		CS(cs),
		selectionBoxTextureID(0)
	{
		font1 = new SolengineV2::Font("Fonts/Hind/Hind-Medium.ttf", 32);
		sb = font1->GetSpriteBatch();
	}
	~GraphicsSystem() 
	{
		delete font1;
		delete sb;
	}


	void AddComponent(int handle, std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram, GraphicsType graphicsType, GLuint64 textID, SolengineV2::Colour col)
	{
		switch (graphicsType)
		{
		case GraphicsType::GENERAL: WorldGraphics.try_emplace(handle, GraphicsComponent(TS->GetLast(), shaderProgram, textID, col)); break;
		case GraphicsType::WORLDUI: WorldUIGraphics.try_emplace(handle, GraphicsComponent(TS->GetLast(), shaderProgram, textID, col)); break;
		case GraphicsType::UI: UIGraphics.try_emplace(handle, GraphicsComponent(TS->GetLast(), shaderProgram, textID, col)); break;
		}
	}

	SolengineV2::Font* font1;
	SolengineV2::SpriteBatch* sb;

	void SetSelectionBoxTextureID(GLint sbtID)
	{
		selectionBoxTextureID = sbtID;
	}

	void SetColour(int handle, SolengineV2::Colour col = { 0, 150, 0, 155 })
	{
		auto it = WorldGraphics.find(handle);
		if (it != WorldGraphics.end())
		{
			it->second.colour = col;
		}
	}

	void Process()
	{
		int sw = CS->GetScreenWidth();
		int sh = CS->GetScreenHeight();
		TransformComponent* CamTransform = CS->cameraTransform.get();
		if (CamTransform == nullptr) return;
		float scale = CS->GetActiveCamScale();

		SpriteBatch.Begin();
		renderWorldComponents(WorldGraphics, CamTransform, sw, sh, scale);
		renderWorldComponents(WorldUIGraphics, CamTransform, sw, sh, scale);
		SpriteBatch.End();
		SpriteBatch.RenderSpriteBatch();

		UIBatch.Begin();
		CS->ProcessUICamera();
		//renderUIComponents(UIGraphics, sw, sh);
		UIBatch.End();
		UIBatch.RenderSpriteBatch();

		sb = font1->GetSpriteBatch();
		sb->Begin();
		font1->Draw("weewoo", { -0.1f, 0.0f }, { 0.5f, 0.5f }, 0.0f, { 0, 255, 255, 255 });
		sb->End();
		sb->RenderSpriteBatch();

		CS->UnuseShader();
	}

	void SetVisibility(int handle, bool set)
	{
		auto graphicsIt = WorldUIGraphics.find(handle);
		if (graphicsIt != WorldUIGraphics.end())
		{
			graphicsIt->second.IsVisible = set;
		}
	}

	void DeleteComponent(int handle)
	{
		WorldGraphics.erase(handle);
		WorldUIGraphics.erase(handle);
		UIGraphics.erase(handle);
	}

private:
	void renderUIComponents(std::map<int, GraphicsComponent> uiGraphics, int sw, int sh)
	{
		for (auto it = uiGraphics.begin(); it != uiGraphics.end(); ++it)
		{
			TransformComponent* TC = it->second.transform.get();
			glm::vec3 pos = TS->GetPos(TC);
			glm::vec3 dims = TS->GetDims(TC);
			const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
			glm::vec4 destRect
			{
				pos.x - dims.x / 2.0f,
				pos.y - dims.y / 2.0f,
				dims.x,
				dims.y
			};

			UIBatch.Draw(destRect, uvRect, it->second.textureID, 0.0f, it->second.colour);
		}
	}

	void renderWorldComponents(std::map<int, GraphicsComponent> graphics, TransformComponent* CamTransform, int sw, int sh, float scale)
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
					CS->ProcessWorldCamera(sp);
				}

				glm::vec3 pos = TS->GetPos(TC);
				glm::vec3 dims = TS->GetDims(TC);

				const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
				glm::vec4 destRect
				{
				pos.x - dims.x / 2.0f,
				pos.y - dims.y / 2.0f,
				dims.x,
				dims.y
				};

				// for drawing selection box
				if (TS->GetIsSelected(TC))
				{
					glm::vec4 dr
					{
						pos.x - dims.x,
						pos.y - dims.y,
						dims.x * 2.0f,
						dims.y * 2.0f
					};

					SpriteBatch.Draw(dr, uvRect, selectionBoxTextureID, 0.0f, SolengineV2::Colour{ 255, 255, 255, 255 });
				}

				SpriteBatch.Draw(destRect, uvRect, it->second.textureID, 0.0f, it->second.colour);
			}
		}
	}

	//frustum culling
	bool isInView(TransformComponent* obj, TransformComponent* cam, int sw, int sh, float scale)
	{
		glm::vec3 pos = TS->GetPos(obj);
		glm::vec3 dims = TS->GetDims(obj);

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