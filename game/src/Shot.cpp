#include "Shot.h"

Shot::Shot()
{
	Radius = 0.08f;
}

void Shot::Draw()
{
	LineModel::Draw();
}

bool Shot::Initialize(float windowWidth, float windowHeight)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	Enabled = false;

	return false;
}

void Shot::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	LifeTimer.Update(deltaTime);
	CheckScreenEdge();

	if (LifeTimer.Elapsed())
	{
		Enabled = false;
	}
}

void Shot::Spawn(Vector3 pos, Vector3 vel, float timerAmount)
{
	Position = pos;
	Velocity = vel;
	Enabled = true;

	LifeTimer.Reset(timerAmount);
}