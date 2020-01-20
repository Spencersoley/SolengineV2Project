#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include <map>

enum class TransformState { ACTIVE, INACTIVE, DELETE };

struct TransformComponent
{
	TransformComponent(glm::vec3 pos, glm::vec3 dims) :
		Pos(pos),
		Dims(dims),
		IsUpdated(true),
	    state(TransformState::ACTIVE) {}

	TransformState state;
	glm::vec3 Pos;
	glm::vec3 Dims;
	std::map<int, std::shared_ptr<TransformComponent>> Children{};
	bool IsUpdated;
};

class TransformSystem //Perhaps this is simply a gameobject manager?
{
public:
	std::map<int, std::shared_ptr<TransformComponent>> Transforms{};
	std::shared_ptr<TransformComponent> LastTransform; // Speeds up creation if utilised correctly

	std::vector<int> handlesToDelete;

	void AddComponent(int handle, glm::vec3 pos, glm::vec3 dims)
	{
		std::shared_ptr<TransformComponent> temp = std::make_shared<TransformComponent>(pos, dims);
		Transforms[handle] = temp;
		LastTransform = temp;
	}

	bool AddChild(int parentHandle, int childHandle, glm::vec3 pos, glm::vec3 dims)
	{
		return AddChild(parentHandle, childHandle, Transforms, pos, dims);
	}

	bool AddChild(int parentHandle, int childHandle, std::map<int, std::shared_ptr<TransformComponent>>& map, glm::vec3 pos, glm::vec3 dims)
	{
		for (auto i = map.begin(); i != map.end(); ++i)
		{
			if (i->first == parentHandle)
			{
				std::shared_ptr<TransformComponent> temp = std::make_shared<TransformComponent>(i->second->Pos + pos, dims);
				i->second->Children[childHandle] = temp;
				LastTransform = temp;
				return true;
			}
			else
			{
				if (AddChild(parentHandle, childHandle, i->second->Children, pos, dims)) return true;
			}
		}

		return false;
	}

	std::shared_ptr<TransformComponent> GetLast()
	{
		return LastTransform;
	}

	std::shared_ptr<TransformComponent> Get(int handle)
	{
		return Get(handle, Transforms);
	}

	std::shared_ptr<TransformComponent> Get(int handle, std::map<int, std::shared_ptr<TransformComponent>> map)
	{
		for (auto i = map.begin(); i != map.end(); ++i)
		{
			if (i->first == handle)
			{
				return i->second;
			}
			else
			{
				std::shared_ptr<TransformComponent> t = Get(handle, i->second->Children);
				if (t != nullptr) return t;
			}
		}

		return nullptr;
	}

	void SetToBeDeleted(int handle) // ACTUALLY deletes it from tree, and sets handles to be deleted
	{
		Delete(handle, Transforms);
	}

	bool Delete(int handle, std::map<int, std::shared_ptr<TransformComponent>>& map) //finds transform by handle
	{
		for (auto i = map.begin(); i != map.end(); ++i)
		{
			if (i->first == handle) // found the handle
			{
				DeleteChildren(i->second);
				map.erase(i);
				handlesToDelete.push_back(handle);
				return true; // deletion complete, break the loop
			}
			else
			{
				if (Delete(handle, i->second->Children)) return true; // completion will propagate, otherwise continues 
			}
		}

		return false;
	}

	void DeleteChildren(std::shared_ptr<TransformComponent> transform) //deletes transform's children
	{
		for (auto it = transform->Children.begin(); it != transform->Children.end();)
		{
			DeleteChildren(it->second);
			handlesToDelete.push_back(it->first);
			it = transform->Children.erase(it);
		}
	}

	///

	///
	void slice(TransformComponent* TC, float sliceSize)
	{
		float leftSide = TC->Pos.x - (TC->Dims.x / 2.0f);

		TC->Dims.x *= sliceSize;
		TC->Pos.x = leftSide + TC->Dims.x / 2.0f;
	}

	void Translate(TransformComponent* TC, glm::vec2 transVec)
	{
		TC->Pos.x += transVec.x;
		TC->Pos.y += transVec.y;
		TC->IsUpdated = true;

		for (auto it = TC->Children.begin(); it != TC->Children.end(); ++it)
		{
			Translate(it->second.get(), transVec);
		}
	}
};
