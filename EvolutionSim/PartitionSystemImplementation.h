#pragma once
#include "PartitionSystemInterface.h"
#include "SpatialPartitionSystem.h"

namespace Partition
{
	using SystemImplementation = SpatialPartitionSystem;
	using System = PartitionSystemInterface<SystemImplementation>;
};
