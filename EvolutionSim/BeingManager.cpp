#pragma once
#include "BeingManager.h"
#include "GeneEnum.h"

using Trait = Gene::Trait;
using BeingType = Gene::BeingType;

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