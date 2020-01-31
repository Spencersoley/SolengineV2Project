#include <map>
#include "TransformSystem.h"
class SpatialPartitionSystem
{
	TransformSystem* TS;

	void init(TransformSystem* ts)
	{
		TS = ts;
	}

	std::map<int, std::map<int, std::vector<std::shared_ptr<TransformComponent>>>> transformGrid;


	void Update()
	{
		transformGrid.clear();
		const int gridSize = 20;
		for (auto it = TS->Transforms.begin(); it != TS->Transforms.end(); ++it)
		{
			glm::vec3 pos = TS->GetPos(it->second.get());
			transformGrid[pos.x / gridSize][pos.y / gridSize].push_back(it->second);
		}
	}
};