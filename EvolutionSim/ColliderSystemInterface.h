#pragma once
#include <set>

class BeingManager;
class PartitionComponent;
class GenerationManager;
class TextureLibrary;

template <class Implementation>
class ColliderSystemInterface
{
	using Handle = unsigned int;
public:
	static void update(BeingManager& beings, PartitionComponent& partition, GenerationManager& generationManager, std::set<Handle>& handlesToDelete, unsigned int dt, TextureLibrary& textureLibrary)
	{
		static Implementation system;
		system.update(beings, partition, generationManager, handlesToDelete, dt, textureLibrary);
	}
};