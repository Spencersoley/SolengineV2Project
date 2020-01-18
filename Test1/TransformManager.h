#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <algorithm>
#include <memory>
#include <iostream>

struct TransformComponent
{
	TransformComponent(glm::vec3 pos, glm::vec3 dims) :
		Pos(pos),
		Dims(dims),
		IsUpdated(true),
		deletable(false),
		toBeDeleted(false) {}

	glm::vec3 Pos;
	glm::vec3 Dims;
	bool IsUpdated;
private:
	friend class TransformManager;
	bool deletable;
	bool toBeDeleted;
};

class TransformManager //Perhaps this is simply a gameobject manager?
{
public:
	std::vector<std::shared_ptr<TransformComponent>> Transforms{};

	//std::map<int, std::shared_ptr<TransformComponent>> TransformMap{};

	int count = 0;

	void AddComponent(glm::vec3 pos, glm::vec3 dims)
	{
		Transforms.push_back(std::make_shared<TransformComponent>(pos, dims));
		//TransformMap[count++] = std::make_shared<TransformComponent>(pos, dims)
	}

	void Process() //delete transforms
	{
		for (auto it = Transforms.begin(); it != Transforms.end();)
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

	std::shared_ptr<TransformComponent> GetLastTransform()
	{
		return Transforms[Transforms.size() - 1];
	}

	void Translate(TransformComponent* TC, glm::vec2 transVec)
	{
		TC->Pos.x += transVec.x;
		TC->Pos.y += transVec.y;
		TC->IsUpdated = true;
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
