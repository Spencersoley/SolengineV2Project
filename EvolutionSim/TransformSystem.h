#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include <map>
#include "MakeSharedEnabler.h"

enum class TransformState : unsigned int { ACTIVE, INACTIVE, DELETE };

class TransformComponent
{
private:
	friend class TransformSystem;
	TransformComponent(glm::vec3 pos, glm::vec3 dims)
		:
		Pos(pos),
		Dims(dims),
		IsUpdated(true),
		IsSelected(false),
		state(TransformState::ACTIVE),
		IsVisible(true)
	{}

	TransformState state;
	glm::vec3 Pos;
	glm::vec3 Dims;
	std::map<int, std::shared_ptr<TransformComponent>> Children{};
	bool IsUpdated;
	bool IsSelected;
	bool IsVisible;
};

class TransformSystem //Perhaps this is simply a gameobject manager?
{
	std::map<int, std::shared_ptr<TransformComponent>> transformMap{};
	std::shared_ptr<TransformComponent> lastTransform;
	std::vector<int> handlesToDelete;

public:

	std::shared_ptr<TransformComponent> GetLastTransform() { return lastTransform; } //creation optimisation
	std::vector<int>::iterator GetHandlesToDeleteBegin() { return handlesToDelete.begin(); } //is there any merit to only exposing begin/end iterators?
	std::vector<int>::iterator GetHandlesToDeleteEnd() { return handlesToDelete.end(); }
	void ClearHandles() { handlesToDelete.clear(); }

	void AddComponent(int handle, glm::vec3 pos, glm::vec3 dims)
	{
		std::shared_ptr<TransformComponent> temp (new TransformComponent(pos, dims));// makeSharedComponent<TransformComponent>(pos, dims);
		transformMap.insert({ handle, temp }); //prefer insertion for shared_ptrs
		lastTransform = temp;
	}

	bool AddChild(int parentHandle, int childHandle, glm::vec3 pos, glm::vec3 dims)
	{
		return AddChild(parentHandle, childHandle, transformMap, pos, dims);
	}

	bool AddChild(int parentHandle, int childHandle, std::map<int, std::shared_ptr<TransformComponent>>& map, glm::vec3 pos, glm::vec3 dims)
	{
		for (auto i = map.begin(); i != map.end(); ++i)
		{
			if (i->first == parentHandle)
			{
			    std::shared_ptr<TransformComponent> temp (new TransformComponent (i->second->Pos + pos, dims));
				i->second->Children.insert(std::pair<int, std::shared_ptr<TransformComponent>>(childHandle, temp));
				lastTransform = temp;
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
		return lastTransform;
	}

	std::shared_ptr<TransformComponent> Get(int handle)
	{
		return Get(handle, transformMap);
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
		Delete(handle, transformMap);
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
	void Slice(TransformComponent* TC, float sliceSize)
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

	glm::vec3 GetPos(TransformComponent* TC)
	{
		return TC->Pos;
	};

	float GetDistFromCentre(TransformComponent* TC)
	{
		return sqrt((TC->Pos.x * TC->Pos.x) + (TC->Pos.y * TC->Pos.y));
	}

	glm::vec3 GetDims(TransformComponent* TC)
	{
		return TC->Dims;
	};

	bool GetIsUpdated(TransformComponent* TC)
	{
		return TC->IsUpdated;
	}

	bool GetIsSelected(TransformComponent* TC)
	{
		return TC->IsSelected;
	}

	// Conditional visibility component??
	bool GetVisibility(TransformComponent* TC)
	{
		return TC->IsVisible;
	}

	void SetVisibility(TransformComponent* TC, bool set)
	{
		TC->IsVisible = set;
	}

	void SetIsSelected(TransformComponent* TC, bool set)
	{
		TC->IsSelected = set;
	}
};
