#include "Shot.h"

Shot::Shot(float windowWidth, float windowHeight)
{
	Enabled = false;
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	Radius = 0.08f;
	LifeTimer = new Timer();
}

void Shot::Draw()
{
	LineModel::Draw();
}

void Shot::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	LifeTimer->Update(deltaTime);
	CheckScreenEdge();

	if (LifeTimer->Elapsed())
	{
		Enabled = false;
	}
}

void Shot::Spawn(Vector3 pos, Vector3 vel, float timerAmount)
{
	Position = pos;
	Velocity = vel;
	Enabled = true;

	LifeTimer->Reset(timerAmount);
}