#pragma once
#include "PositionedObject.h"

class Entity : public PositionedObject
{
public:
	bool Enabled = true;
	bool BeenHit = false;
	bool HasCollision = true;
	bool Debug = false;
	float Scale = 1;

	Sound Sound01;
	Sound Sound02;
	Sound Sound03;
	Sound Sound04;

	virtual void Update(float deltaTime);
	virtual void Draw();

	bool CirclesIntersect(Entity* target);

private:
};

