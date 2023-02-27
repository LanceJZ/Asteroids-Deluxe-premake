#include "RockData.h"

RockData::RockData()
{
}

RockData::~RockData()
{
}

void RockData::SetVelocity(Vector3 velocity)
{
	Velocity = velocity;
}

void RockData::Update(Vector3 position, bool enabled)
{
	Position = position;
	Enabled = enabled;
}