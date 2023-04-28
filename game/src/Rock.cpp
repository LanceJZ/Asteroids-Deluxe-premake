#include "Rock.h"

Rock::Rock(float windowWidth, float windowHeight, Player* player, UFO* ufo, Color TheColor)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	Rock::ModelColor = TheColor;
	Rock::ThePlayer = player;
	Rock::TheUFO = ufo;
	LineModel::ModelColor = TheColor;
}

Rock::~Rock()
{
}

void Rock::SetDotModel(vector<Vector3> dotModel)
{
	Rock::DotModel = dotModel;
}

void Rock::LoadSound(Sound exp)
{
	Sound01 = exp;
	SetSoundVolume(Sound01, 0.5f);
}

bool Rock::Initialize()
{
	TheExploder.Initialize(DotModel, ModelColor);

	return false;
}

void Rock::Update(float deltaTime)
{
	LineModel::Update(deltaTime);
	TheExploder.Update(deltaTime);
	CheckScreenEdge();

	if (CheckCollision())
	{
		BeenHit = true;
		Enabled = false;

		if (!ThePlayer->GameOver)
		{
			PlaySound(Sound01);
		}

		TheExploder.Spawn(Position, 15, Radius);
	}
}

void Rock::Draw()
{
	LineModel::Draw();
	TheExploder.Draw();
}

void Rock::Spawn(Vector3 pos, float speed, RockSize size)
{
	float magnitude = GetRandomFloat(1.1f, 5.1f);
	float angle = GetRandomRadian();
	Vector3 dir = {cos(angle) * magnitude, sin(angle) * magnitude};
	float maxVS = 0;

	Position = pos;
	Velocity = dir;
	Rock::Size = size;
	float scale = 1.00666f;

	switch (size)
	{
	case Small:
		Scale = scale / 3.5f;
		Radius = 2.10f / 3.5f;
		maxVS = 3;
		break;
	case Medium:
		Scale =  scale / 2;
		Radius = 2.10f / 2;
		maxVS = 2;
		break;
	case Large:
		Scale = scale;
		Radius = 2.10f;
		maxVS = 1;

		if (Velocity.x > 0)
		{
			X(-WindowWidth);
		}
		else
		{
			X(WindowWidth);
		}

		break;
	}

	float rotVelSpeed = GetRandomFloat(-maxVS, maxVS);
	RotationVelocity = rotVelSpeed;
	Enabled = true;
	BeenHit = false;
}

void Rock::GiveScore()
{
	switch (Size)
	{
	case Large:
		ThePlayer->ScoreUpdate(20);
		break;
	case Medium:
		ThePlayer->ScoreUpdate(50);
		break;
	case Small:
		ThePlayer->ScoreUpdate(100);
		break;
	}
}

bool Rock::CheckCollision()
{
	for (auto &shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(&shot))
		{
			shot.Enabled = false;
			GiveScore();

			return true;
		}
	}

	if (CirclesIntersect(ThePlayer))
	{
		if (!ThePlayer->ShieldHit(Position, Velocity))
		{
			GiveScore();
			return true;
		}
	}

	if (CirclesIntersect(TheUFO))
	{
		TheUFO->Enabled = false;

		return true;
	}

	if (CirclesIntersect(&TheUFO->TheShot))
	{
		TheUFO->TheShot.Enabled = false;

		return true;
	}

	return false;
}
