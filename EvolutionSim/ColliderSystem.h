#pragma once
#include <set>

class BeingManager;
class Being;
class PartitionComponent;
class TextureLibrary;

class ColliderSystem
{
	using Handle = unsigned int;

public:
	void update(BeingManager& beings, PartitionComponent& partition, GenerationManager& generationManager, std::set<Handle>& handlesToDelete, unsigned int dt, TextureLibrary& textureLibrary);

private:
	void detectCollision(Being& beingA, Being& beingB, const Handle handleA, const Handle handleB, const float foodMultiplier, std::set<Handle>& handlesToDelete, TextureLibrary& textureLibrary);

	void eatOnCollision(float foodMultiplier, Being& beingA, Being& beingB, const Handle handleB, std::set<Handle>& handlesToDelete, TextureLibrary& textureLibrary);
};