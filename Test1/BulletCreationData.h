#pragma once
#include "Team.h"

struct BulletCreationData
{
	BulletCreationData(glm::vec3 p, glm::vec2 d, float v, Team t) : pos(p), dir(d), vel(v), team(t) {}

	glm::vec3 pos;
	glm::vec2 dir;
	Team team;
	float vel;
};
