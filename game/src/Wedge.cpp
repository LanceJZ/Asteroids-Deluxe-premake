#include "Wedge.h"

Wedge::Wedge(float windowWidth, float windowHeight, Player* player, UFO* ufo, CrossCom* crossCom, Color color)
{
	WindowHeight = windowHeight;
	WindowWidth = windowWidth;
	Wedge::ThePlayer = player;
	Wedge::TheUFO = ufo;
	Wedge::Comm = crossCom;
	Wedge::ModelColor = color;
	Radius = 0.72f;
}

Wedge::~Wedge()
{
	UnloadSound(Sound01);
}

bool Wedge::Initialize()
{
	TurnOff();

	return false;
}

void Wedge::LoadSound(Sound explode)
{
	Sound01 = explode;
	SetSoundVolume(Sound01, 0.75f);
}

void Wedge::Input()
{

}

void Wedge::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	if (!Docked)
	{
		if (!Comm->NewWave)
		{
			if (ThePlayer->Enabled)
			{
				ChasePlayer();
			}
			else if (TheUFO->Enabled)
			{
				ChaseUFO();
			}
			else
			{
				LeavePlay();
			}

			CheckScreenEdge();
		}
		else
		{
			LeavePlay();
		}

		if (CheckCollision())
		{
			Collision();
		}
	}
}

void Wedge::Draw()
{
	LineModel::Draw();
}

void Wedge::Spawn()
{
	Enabled = true;
	Docked = true;
}

bool Wedge::CheckCollision()
{
	if (CirclesIntersect(ThePlayer))
	{
		if (!ThePlayer->ShieldHit(Position, Velocity))
		{
			ThePlayer->ScoreUpdate(TheScore);
			return true;
		}
	}

	for (auto shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(shot))
		{
			ThePlayer->ScoreUpdate(TheScore);
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

void Wedge::Collision()
{
	if (!ThePlayer->GameOver)
		PlaySound(Sound01);

	TurnOff();
}

void Wedge::ChasePlayer()
{
	RotateVelocity(ThePlayer->Position, TurnSpeed, Speed);
}

void Wedge::ChaseUFO()
{
	RotateVelocity(TheUFO->Position, TurnSpeed, Speed);
}

void Wedge::TurnOff()
{
	Enabled = false;
	Velocity = { 0 };
	RotationVelocity = 0;
	Position = { 30, 30, 0 };
}

void Wedge::LeavePlay()
{
	PositionedObject::LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		Initialize();
	}
}