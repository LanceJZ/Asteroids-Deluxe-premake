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

	Sound Sound01 = { 0 };
	Sound Sound02 = { 0 };
	Sound Sound03 = { 0 };
	Sound Sound04 = { 0 };

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual bool CirclesIntersect(Entity* target);

private:
};

