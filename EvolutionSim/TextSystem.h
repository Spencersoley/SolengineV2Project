#pragma once
#include <memory>
#include <SpriteBatch.h>
#include <Font.h>
#include "TransformSystem.h"
#include "CameraSystem.h"

class TextComponent
{
private:
	friend class TextSystem;
	TextComponent(
		std::shared_ptr<TransformComponent> _transform,
		SolengineV2::Colour _colour,
		std::string _text
	)
		:
		transform(_transform),
		colour(_colour),
		text(_text)
	{	
	
	}

	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram;
	SolengineV2::Colour colour;
	std::string text;
};

class TextSystem
{
	TransformSystem* transformSystem;
	CameraSystem* cameraSystem;
	std::shared_ptr<SolengineV2::ShaderProgram> shaderProgram;

	SolengineV2::Font* font1;
	SolengineV2::SpriteBatch* spriteBatch;

	std::map<int, TextComponent> textMap{};

public:
	TextSystem(
		TransformSystem* _transformSystem,
		CameraSystem* _cameraSystem,
		std::shared_ptr<SolengineV2::ShaderProgram> _shaderProgram
	)
		:
		transformSystem(_transformSystem),
		cameraSystem(_cameraSystem),
		shaderProgram(_shaderProgram)
	{
		font1 = new SolengineV2::Font("Fonts/Hind/Hind-Medium.ttf", 32);
		spriteBatch = font1->GetSpriteBatch();
	}
	~TextSystem()
	{
		delete font1;
	}

	void AddComponent(int handle, SolengineV2::Colour col, std::string initialString)
	{
		textMap.try_emplace(handle, TextComponent(transformSystem->GetLast(), col, initialString));
	}

	void Process()
	{
		cameraSystem->ProcessUICamera();

		spriteBatch->Begin();

		for (auto tc : textMap)
		{
			font1->Draw(tc.second.text.c_str(), 
				{ transformSystem->GetPos(tc.second.transform.get()).x, transformSystem->GetPos(tc.second.transform.get()).y }, 
				{ transformSystem->GetDims(tc.second.transform.get()).x, transformSystem->GetDims(tc.second.transform.get()).y }, 
				0.0f, 
				tc.second.colour);
		}

		spriteBatch->End();
		spriteBatch->RenderSpriteBatch();

		cameraSystem->UnuseShader();
	}

	void UpdateText(int handle, std::string str) 
	{
		auto it = textMap.find(handle);
		if (it != textMap.end()) it->second.text = str;
	}

	void DeleteComponent(int handle)
	{
		textMap.erase(handle);
	}
};