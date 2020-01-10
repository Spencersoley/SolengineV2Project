#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include "TransformComponent.h"

class TransformManager
{
public:
	TransformManager() {}

	std::vector<std::shared_ptr<TransformComponent>> Transforms{};

	void Translate(TransformComponent* TC, glm::vec2 transVec)
	{
		TC->Pos.x += transVec.x;
		TC->Pos.y += transVec.y;
		TC->IsUpdated = true;
	}

	void AddComponent(glm::vec3 pos, glm::vec3 dims)
	{
		Transforms.push_back(std::make_shared<TransformComponent>(pos, dims));
	}

	std::shared_ptr<TransformComponent> GetLastTransform()
	{
		return Transforms[Transforms.size() - 1];
	}

	void Process() //delete transforms
	{
		for (auto it = Transforms.begin(); it != Transforms.end(); )
		{
			if (IsToBeDeleted(it->get()))
			{
				std::cout << "TRANSF----------USE COUNT: " << it->use_count() << std::endl;
				it = Transforms.erase(it);
				std::cout << "Transform size: " << Transforms.size() << std::endl;
			}
			else
			{
				++it;
			}
		}
	}

	void SetDeletable(TransformComponent* TC)
	{
		TC->deletable = true;
	}

	void SetDeletable()
	{
		GetLastTransform()->deletable = true;
	}

	void SetToBeDeleted(TransformComponent* TC)
	{
		if (TC->deletable)
		{
			TC->toBeDeleted = true;
		}
	}

	bool IsDeletable(TransformComponent* TC)
	{
		return TC->deletable;
	}

	bool IsToBeDeleted(TransformComponent* TC)
	{
		return TC->toBeDeleted;
	}
};
