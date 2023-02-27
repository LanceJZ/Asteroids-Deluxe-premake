#include "WedgeGroup.h"
#include "raymath.h"

WedgeGroup::WedgeGroup(float windowWidth, float windowHeight, Player* player, UFO* ufo, CrossCom* crossCom, Color color)
{
	for (int i = 0; i < 3; i++)
	{
		WedgePairs[i] = new WedgePair(windowWidth, windowHeight, player, ufo, crossCom, color);
	}

	WindowHeight = windowHeight;
	WindowWidth = windowWidth;
	WedgeGroup::ThePlayer = player;
	WedgeGroup::TheUFO = ufo;
	WedgeGroup::Comm = crossCom;
	WedgeGroup::TheColor = color;

	Radius = 1.6f;
}

WedgeGroup::~WedgeGroup()
{
	UnloadSound(Sound01);
}

bool WedgeGroup::Initialize()
{
	//float wY = 0.75f;
	//float wYlower = 0.35f;
	//float wX = 0.65f;
	//float rot = 0.333f;

	//WedgePairs[0]->Rotation = 0;
	//WedgePairs[0]->Position.y = wY;
	//WedgePairs[0]->Position.x = 0;
	//WedgePairs[1]->Position = { wX, -wYlower, 0 };
	//WedgePairs[1]->Rotation = (float)PI * rot;
	//WedgePairs[2]->Position = { -wX, -wYlower, 0 };
	//WedgePairs[2]->Rotation = ((float)PI * 2) * rot;

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->Initialize();
		//AddChild(wedgePair);
	}

	Enabled = false;
	Comm->NewWave = false;
	Position = { 50, 50, 0 };

	return false;
}

void WedgeGroup::LoadSound(Sound explode)
{
	Sound01 = explode;
	SetSoundVolume(Sound01, 0.75f);

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->LoadSound(explode);
	}
}

void WedgeGroup::LoadModel(string model)
{
	WedgeModel.LoadModel(model);

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->LoadWedgeModel(WedgeModel.GetModel());
	}
}

void WedgeGroup::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (WedgepairsDocked)
	{
		if (CheckCollision())
			Collision();

		float wY = 0.75f;
		float wYlower = 0.35f;
		float wX = 0.65f;
		float rot = 0.333f;

		Comm->WedgeGroupPos = Position;

		WedgePairs[0]->Position.y = Position.y + wY;
		WedgePairs[0]->Position.x = Position.x;
		WedgePairs[1]->Position = { Position.x + wX, Position.y - wYlower, 0 };
		WedgePairs[1]->Rotation = (float)PI * rot;
		WedgePairs[2]->Position = { Position.x - wX, Position.y - wYlower, 0 };
		WedgePairs[2]->Rotation = ((float)PI * 2) * rot;

		if (!Comm->NewWave)
		{
			if (Enabled)
				CheckScreenEdge();
		}
		else
		{
			if (OffScreen())
			{
				Initialize();
			}
		}
	}

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->Update(deltaTime);
	}
}

void WedgeGroup::Draw()
{
	Entity::Draw();

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->Draw();
	}
}

void WedgeGroup::Spawn(Vector3 position, Vector3 velocity)
{
	Initialize();

	Comm->WedgeGroupActive = true;
	Enabled = true;
	WedgepairsDocked = true;
	Position = position;
	Velocity = velocity;
	float rot = 0.333333f;
	WedgePairs[0]->Rotation = 0;
	WedgePairs[1]->Rotation = (float)PI * rot;
	WedgePairs[2]->Rotation = ((float)PI * 2) * rot;

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->Spawn();
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

	for (auto shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(shot))
		{
			ThePlayer->ScoreUpdate(Score);
			shot->Enabled = false;
			return true;
		}
	}

	if (CirclesIntersect(TheUFO))
	{
		TheUFO->Collision();
		return true;
	}

	if (CirclesIntersect(TheUFO->TheShot))
	{
		TheUFO->TheShot->Enabled = false;
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
	Velocity = { 0 };
	Enabled = false;
	Comm->WedgeGroupActive = false;

	for (auto wedgePair : WedgePairs)
	{
		wedgePair->GroupDocked = false;
	}
}