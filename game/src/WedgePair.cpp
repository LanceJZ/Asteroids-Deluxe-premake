#include "WedgePair.h"

WedgePair::WedgePair()
{
}

WedgePair::~WedgePair()
{
}

bool WedgePair::Initialize(float windowWidth, float windowHeight, Player* player,
	UFO* ufo, CrossCom* crossCom, Color TheColor)
{
	for (int i = 0; i < 2; i++)
	{
		Wedges[i].Initialize(windowWidth, windowHeight, player, ufo, crossCom, TheColor);
	}

	WindowHeight = windowHeight;
	WindowWidth = windowWidth;
	WedgePair::ThePlayer = player;
	WedgePair::TheUFO = ufo;
	WedgePair::Comm = crossCom;
	WedgePair::TheColor = TheColor;

	Radius = 0.95f;

	Reset();
	TurnOff();

	return false;
}

void WedgePair::LoadWedgeModel(vector<Vector3> model)
{
	WedgeModel.SetModel(model);
	SetSoundVolume(Sound01, 0.75f);

	for (auto &wedge : Wedges)
	{
		wedge.SetModel(WedgeModel.GetModel());
	}
}

void WedgePair::SetDebug()
{
	Wedges[0].Debug = true;
	Wedges[1].Debug = true;
}

void WedgePair::LoadSound(Sound explode)
{
	Sound01 = explode;

	for (auto &wedge : Wedges)
	{
		wedge.LoadSound(explode);
	}
}

void WedgePair::LoadModel(string model)
{
}

void WedgePair::Input()
{

}

void WedgePair::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	for (auto &wedge : Wedges)
	{
		wedge.Update(deltaTime);
	}

	if (WedgeDocked) //Still together in a pair.
	{
		Vector3 pos = VelocityFromAngleZ(Rotation, 0.65f);

		Wedges[1].X(Position.x - pos.x);
		Wedges[1].Y(Position.y - pos.y);
		Wedges[0].X(pos.x + Position.x);
		Wedges[0].Y(pos.y + Position.y);
		Wedges[1].Rotation = (float)PI + Rotation;
		Wedges[0].Rotation = Rotation;

		if (!GroupDocked) //Not in the group.
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
}

void WedgePair::Draw()
{
	Entity::Draw();

	for (auto &wedge : Wedges)
	{
		wedge.Draw();
	}
}

void WedgePair::Spawn()
{
	Enabled = true;
	WedgeDocked = true;
	GroupDocked = true;
	Velocity = { 0 };
	RotationVelocity = 0;

	for (auto &wedge : Wedges)
	{
		wedge.Spawn();
	}
}

bool WedgePair::CheckCollision()
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

void WedgePair::Collision()
{
	if (!ThePlayer->GameOver)
		PlaySound(Sound01);

	WedgeDocked = false;
	Enabled = false;

	for (auto &wedge : Wedges)
	{
		wedge.Docked = false;
	}
}

void WedgePair::TurnOff()
{
	Enabled = false;
	Position = { 30, 30, 0 };
}

void WedgePair::ChasePlayer()
{
	RotateVelocity(ThePlayer->Position, TurnSpeed, Speed);
}

void WedgePair::ChaseUFO()
{
	RotateVelocity(TheUFO->Position, TurnSpeed, Speed);
}

void WedgePair::LeavePlay()
{
	PositionedObject::LeavePlay(TurnSpeed, Speed);

	if (OffScreen())
	{
		TurnOff();
	}
}

void WedgePair::Reset()
{
}
