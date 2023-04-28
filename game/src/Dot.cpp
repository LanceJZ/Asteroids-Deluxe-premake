#include "Dot.h"
#include "raymath.h"

bool Dot::Initialize()
{
	TheTimer = new Timer();
	Scale = 0.5f;

	return false;
}

void Dot::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (Enabled)
	{
		TheTimer->Update(deltaTime);

		if (TheTimer->Elapsed())
		{
			Enabled = false;
			Position = { NULL };
		}
	}
}

void Dot::Draw()
{
	LineModel::Draw();
}

void Dot::Spawn(Vector3 position, float time, float size)
{
	Position = Vector3Add(position, { GetRandomFloat(-size, size), GetRandomFloat(-size, size),
		GetRandomFloat(-size, size) });

	float speed = 2.0f;

	Velocity = { GetRandomFloat(-speed, speed), GetRandomFloat(-speed, speed),
		GetRandomFloat(-speed, speed) };

	TheTimer->Reset(GetRandomFloat(-time, time));
	Enabled = true;
}

void Dot::Clear()
{
	Enabled = false;
}
