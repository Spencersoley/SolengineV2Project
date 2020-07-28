#pragma once
#include "BeingManager.h"
#include "BeingCreateInfo.h"

#include "TransformSystem.h"
#include "SpriteSystem.h"
#include "GeneSystem.h"
#include "VelocitySystem.h"
#include "SurvivalSystem.h"
#include "Being.h"

using Trait = GeneComponent::Trait;
using BeingType = GeneComponent::BeingType;

size_t BeingManager::getSize() const
{
	return pool.size();
}

void BeingManager::init(const size_t size)
{
	if (size == 0) return;
	pool.reserve(size * sizeof(Being));
}

void BeingManager::deleteBeing(const Handle handle, Handle& selectedHandle)
{
	const size_t size = getSize();
	if (handle >= size)
	{
		throw std::exception("handle outside of being pool range");
	}
	if (selectedHandle == size - 1) selectedHandle = handle;
	if (selectedHandle == handle) selectedHandle = size;

	pool[handle] = pool.back();
	pool.pop_back();
}

void BeingManager::resize(const Handle start)
{
	pool.resize(start);
}

void BeingManager::setToSize(const size_t size)
{
	pool.resize(size);
}

void BeingManager::clear()
{
	pool.clear();
}

void BeingManager::reset()
{
	uint32_t size = getSize();
	clear();
	init(size);
}