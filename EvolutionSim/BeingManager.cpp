#pragma once
#include "BeingManager.h"

uint32_t BeingManager::getSize() const 
{
	return pools.transform.data.size();
}

void BeingManager::init(const uint32_t size)
{
	if (size == 0) return;
	pools.transform.init(size);
	pools.velocity.init(size);
	pools.sprite.init(size);
	pools.survival.init(size);
	pools.gene.init(size);
}

void BeingManager::create(const BeingCreateInfo& createInfo)
{
	pools.transform.insert(createInfo.beingHandle, TransformComponent(createInfo.pos, createInfo.dims));
	pools.sprite.insert(createInfo.beingHandle, SpriteComponent(textureID, createInfo.colour));
	pools.survival.insert(createInfo.beingHandle, SurvivalComponent(createInfo.survivalState, createInfo.isAlive, createInfo.fullness, createInfo.stamina));
	pools.velocity.insert(createInfo.beingHandle, VelocityComponent(0.0f, glm::vec2(0.0f)));
	pools.gene.insert(createInfo.beingHandle, createInfo.genes);
}

void BeingManager::deleteBeing(const uint32_t& handle, uint32_t& selectedHandle)
{
	const uint32_t size = getSize();
	if (handle >= size) throw std::exception("handle outside of being pool range");
	if (selectedHandle == size - 1) selectedHandle = handle;
	if (selectedHandle == handle) selectedHandle = size;
	pools.transform.deleteComponent(handle);
	pools.sprite.deleteComponent(handle);
	pools.survival.deleteComponent(handle);
	pools.velocity.deleteComponent(handle);
	pools.gene.deleteComponent(handle);
}

void BeingManager::resize(const uint32_t start)
{
	pools.transform.data.resize(start);
	pools.sprite.data.resize(start);
	pools.survival.data.resize(start);
	pools.velocity.data.resize(start);
	pools.gene.data.resize(start);
}

void BeingManager::clear()
{
	pools.transform.data.clear();
	pools.sprite.data.clear();
	pools.survival.data.clear();
	pools.velocity.data.clear();
	pools.gene.data.clear();
}

void BeingManager::reset()
{
	uint32_t size = getSize();
	clear();
	init(size);
}
