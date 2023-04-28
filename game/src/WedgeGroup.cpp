#include "WedgeGroup.h"
#include "raymath.h"

WedgeGroup::WedgeGroup()
{
}

WedgeGroup::~WedgeGroup()
{
	UnloadSound(Sound01);
}

bool WedgeGroup::Initialize(float windowWidth, float windowHeight, Player* player,
	UFO* ufo, CrossCom* crossCom, Color TheColor)
{
	for (int i = 0; i < 3; i++)
	{
		WedgePairs[i].Initialize(windowWidth, windowHeight, player, ufo, crossCom, TheColor);
	}

	WindowHeight = windowHeight;
	WindowWidth = windowWidth;
	WedgeGroup::ThePlayer = player;
	WedgeGroup::TheUFO = ufo;
	WedgeGroup::Comm = crossCom;
	WedgeGroup::TheColor = TheColor;
	Position = { 50, 50, 0 };

	Radius = 1.6f;
	Enabled = false;

	return false;
}

void WedgeGroup::LoadSound(Sound explode)
{
	Sound01 = explode;
	SetSoundVolume(Sound01, 0.75f);

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.LoadSound(explode);
	}
}

void WedgeGroup::LoadModel(string model)
{
	WedgeModel.LoadModel(model);

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.LoadWedgeModel(WedgeModel.GetModel());
	}
}

void WedgeGroup::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (WedgepairsDocked)
	{
		if (CheckCollision() && Enabled)
			Collision();

		float wY = 0.75f;
		float wYlower = 0.35f;
		float wX = 0.65f;
		float rot = 0.333f;

		Comm->WedgeGroupPos = Position;

		WedgePairs[0].Position.y = Position.y + wY;
		WedgePairs[0].Position.x = Position.x;
		WedgePairs[1].Position = { Position.x + wX, Position.y - wYlower, 0 };
		WedgePairs[1].Rotation = (float)PI * rot;
		WedgePairs[2].Position = { Position.x - wX, Position.y - wYlower, 0 };
		WedgePairs[2].Rotation = ((float)PI * 2) * rot;

		if (!Comm->NewWave)
		{
			if (Enabled) CheckScreenEdge();
		}
		else
		{
			if (OffScreen())
			{
				Enabled = false;
			}
		}
	}

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.Update(deltaTime);
	}
}

void WedgeGroup::Draw()
{
	Entity::Draw();

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.Draw();
	}
}

void WedgeGroup::Spawn(Vector3 velocity)
{
	Position.y = GetRandomFloat(-WindowHeight, WindowHeight);

	if (velocity.x < 0)
	{
		Position.x = WindowWidth;
	}
	else
	{
		Position.x = -(WindowWidth);
	}

	Comm->NewWave = false;
	Comm->WedgeGroupActive = true;
	Enabled = true;
	WedgepairsDocked = true;
	Velocity = velocity;

	float rot = 0.333333f;
	WedgePairs[0].Rotation = 0;
	WedgePairs[1].Rotation = (float)PI * rot;
	WedgePairs[2].Rotation = ((float)PI * 2) * rot;

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.Spawn();
	}
}

bool WedgeGroup::CheckCollision()
{
	if (CirclesIntersect(ThePlayer))
	{
		if (!ThePlayer->ShieldHit(Position, Velocity))
		{
			ThePlayer->ScoreUpdate(Score);
			return true;
		}
	}

	for (auto &shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(&shot))
		{
			ThePlayer->ScoreUpdate(Score);
			shot.Enabled = false;
			return true;
		}
	}

	if (CirclesIntersect(TheUFO))
	{
		TheUFO->Collision();
		return true;
	}

	if (CirclesIntersect(&TheUFO->TheShot))
	{
		TheUFO->TheShot.Enabled = false;
		return true;
	}

	return false;
}

void WedgeGroup::Collision()
{
	if (!ThePlayer->GameOver)
		PlaySound(Sound01);

	Undock();
}

void WedgeGroup::Undock()
{
	WedgepairsDocked = false;
	Enabled = false;
	Comm->WedgeGroupActive = false;

	for (auto &wedgePair : WedgePairs)
	{
		wedgePair.GroupDocked = false;
	}
}

void WedgeGroup::Reset()
{
}