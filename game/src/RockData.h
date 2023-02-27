#pragma once
#include "raylib.h"

struct RockData
{
	public:
	Vector3 Position;
	Vector3 Velocity;
	bool Enabled;

	RockData();
	~RockData();

	void Update(Vector3 position, bool enabled);
	void SetVelocity(Vector3 velocity);
};

