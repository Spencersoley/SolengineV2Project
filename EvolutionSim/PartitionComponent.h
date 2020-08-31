#pragma once
#include <vector>
#include "Grid.h"

class PartitionComponent
{
	template <typename T> friend class PartitionSystemInterface;
	Grid animalGrid{};
	Grid foodGrid{};
};