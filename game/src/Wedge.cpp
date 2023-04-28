#include "Wedge.h"

Wedge::Wedge()
{
}

Wedge::~Wedge()
{
}

bool Wedge::Initialize(float windowWidth, float windowHeight, Player* player,
	UFO* ufo, CrossCom* crossCom, Color TheColor)
{
	WindowHeight = windowHeight;
	WindowWidth = windowWidth;
	Wedge::ThePlayer = player;
	Wedge::TheUFO = ufo;
	Wedge::Comm = crossCom;
	Wedge::ModelColor = TheColor;
	Radius = 0.72f;

	TurnOff();

	return false;
}

void Wedge::LoadSound(Sound explode)
{
	Sound01 = explode;
	SetSoundVolume(Sound01, 0.75f);
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

		if (CheckCollision() && Enabled)
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
	TurnOff();
	Enabled = true;
	Docked = true;
	Velocity = { 0 };
	RotationVelocity = 0;
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

	for (auto &shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(&shot))
		{
			ThePlayer->ScoreUpdate(TheScore);
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
	Position = { WindowWidth * 2, WindowHeight * 2, 0 };
}

void Wedge::LeavePlay()
{
	PositionedObject::LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		TurnOff();
	}
}

void Wedge::Reset()
{
}
