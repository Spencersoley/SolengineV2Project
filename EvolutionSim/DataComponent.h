#include <vector>

class DataComponent
{
private:
	friend class DataSystem;
	std::vector<float> points{ 0.5f };
};
