#include "UFO.h"
#include "raymath.h"

UFO::UFO(float windowWidth, float windowHeight, Player* player, CrossCom* crossCom, Color color)
{
	UFO::ThePlayer = player;
	UFO::Comm = crossCom;
	UFO::ModelColor = color;
	LineModel::ModelColor = color;
	TheShot = new Shot(windowWidth, windowHeight);
	TheShot->ModelColor = color;
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	FireTimer = new Timer();
	VectorTimer = new Timer();
}

UFO::~UFO()
{
	UnloadSound(Sound01);
	UnloadSound(Sound02);
	UnloadSound(Sound03);
	UnloadSound(Sound04);
}

void UFO::LoadModel(string ship, vector<Vector3> dotModel)
{
	LineModel::LoadModel(ship);
	TheShot->SetModel(dotModel);
	TheExploder = new Exploder(dotModel, ModelColor);
}

void UFO::LoadSound(Sound exp, Sound big, Sound small, Sound fire)
{
	Sound01 = exp;
	Sound02 = big;
	Sound03 = small;
	Sound04 = fire;

	SetSoundVolume(Sound01, 0.5f);
	SetSoundVolume(Sound02, 0.5f);
	SetSoundVolume(Sound03, 0.5f);
	SetSoundVolume(Sound04, 0.5f);
}

bool UFO::Initialise()
{
	Enabled = false;

	return false;
}

void UFO::Update(float deltaTime)
{
	LineModel::Update(deltaTime);
	TheExploder->Update(deltaTime);
	TheShot->Update(deltaTime);

	if (Enabled)
	{
		FireTimer->Update(deltaTime);
		VectorTimer->Update(deltaTime);
		CheckScreenEdgeY();

		if (CheckReachedSide())
		{
			Enabled = false;
		}

		if (VectorTimer->Elapsed())
		{
			ChangeVector();
			ResetVectorTimer();
		}

		if (FireTimer->Elapsed())
		{
			FireShot();
			ResetFireTimer();
		}

		if (!ThePlayer->GameOver)
		{
			switch (size)
			{
			case UFO::Small:
				if (!IsSoundPlaying(Sound03))
				{
					PlaySound(Sound03);
				}
				break;
			case UFO::Large:
				if (!IsSoundPlaying(Sound02))
				{
					PlaySound(Sound02);
				}
				break;
			default:
				break;
			}
		}
	}

	if (CheckCollision())
	{
		Collision();
	}
}

void UFO::Draw()
{
	LineModel::Draw();
	TheExploder->Draw();
	TheShot->Draw();
}

void UFO::Spawn(Vector3 pos, Vector3 vel)
{
	Position = pos;
	Velocity = vel;
	Enabled = true;

	ResetVectorTimer();
	ResetFireTimer();
	ChangeVector();
}

void UFO::Collision()
{
	Enabled = false;
	TheExploder->Spawn(Position, 15, Radius / 2.0f);

	if (!ThePlayer->GameOver)
	{
		PlaySound(Sound01);
		StopSound(Sound02);
		StopSound(Sound03);
	}
}

void UFO::ResetFireTimer()
{
	FireTimer->Reset(GetRandomFloat(0.75f, 2.75f));
}

void UFO::ResetVectorTimer()
{
	VectorTimer->Reset(GetRandomFloat(1.25f, 3.15f));
}

void UFO::ChangeVector()
{
	if (GetRandomValue(1, 10) > 2)
	{
		if ((int)Velocity.y == 0)
		{
			if (GetRandomValue(1, 10) < 5)
			{
				Velocity.y = MaxSpeed;
			}
			else
			{
				Velocity.y = -MaxSpeed;
			}
		}
		else
		{
			if (Position.y < WindowHeight - (Radius * 3) && Position.y > -WindowHeight - (Radius * 3))
			{
				Velocity.y = 0;
			}
		}
	}
}

void UFO::FireShot()
{
	float angle = 0;
	float shotSpeed = 15;
	bool shootRocks = false;

	switch (size)
	{
	case UFO::Large:
		if (GetRandomValue(1, 10) < 5)
		{
			angle = GetRandomRadian();
		}
		else
		{
			if (Comm->WedgeGroupActive)
			{
				angle = AimedShotAtWGroup();
			}
			else
			{
				angle = AimedShot();
			}
		}
		break;
	case UFO::Small:
		if (Comm->WedgeGroupActive)
		{
			angle = AimedShotAtWGroup();
		}
		else
		{
			angle = AimedShot();
		}
		break;
	}

	if (!ThePlayer->Enabled && !Comm->WedgeGroupActive)
	{
		angle = AimedShotAtRock();
	}

	if (!TheShot->Enabled)
	{
		if (!ThePlayer->GameOver)
		{
			PlaySound(Sound04);
		}

		Vector3 offset = Vector3Add(VelocityFromAngleZ(Radius), Position);
		TheShot->Spawn(offset,	VelocityFromAngleZ(angle, shotSpeed), 2.5f);
	}
}

float UFO::AimedShot()
{
	if (!ThePlayer->Enabled)
	{
		return GetRandomRadian();
	}

	float percentChance = 0.2f - (ThePlayer->Score * 0.00001f);

	if (percentChance < 0)
	{
		percentChance = 0;
	}

	percentChance += GetRandomFloat(0.0f, 0.05f);

	return AngleFromVectorZ(ThePlayer->Position) + GetRandomFloat(-percentChance, percentChance);
}

float UFO::AimedShotAtWGroup()
{
	float percentChance = GetRandomFloat(0.0f, 0.05f);

	return AngleFromVectorZ(Comm->WedgeGroupPos) + GetRandomFloat(-percentChance, percentChance);
}

float UFO::AimedShotAtRock()
{
	bool noRocks = true;
	Vector3 closestRockPos = { 0 };
	Vector3 closestRockVel = { 0 };
	float shortistDistance = 200;

	for (auto rock : Rocks)
	{
		if (rock->Enabled)
		{
			noRocks = false;

			float rockDistance = Vector3Distance(Position, rock->Position);

			if (rockDistance < shortistDistance)
			{
				closestRockPos = rock->Position;
				closestRockVel = rock->Velocity;
				shortistDistance = rockDistance;
			}
		}
	}

	if (noRocks)
	{
		return GetRandomRadian();
	}

	return AngleFromVectorZ(Vector3Add(closestRockPos, closestRockVel));
}

void UFO::GiveScore()
{
	switch (size)
	{
	case Large:
		ThePlayer->ScoreUpdate(200);
		break;
	case Small:
		ThePlayer->ScoreUpdate(1000);
		break;
	}
}

bool UFO::CheckCollision()
{
	if (TheShot->CirclesIntersect(ThePlayer))
	{
		ThePlayer->ShieldHit(TheShot->Position, TheShot->Velocity);
		TheShot->Enabled = false;
	}

	if (CirclesIntersect(ThePlayer))
	{
		if (!ThePlayer->ShieldHit(Position, Velocity))
		{
			GiveScore();
			return true;
		}
	}

	for (auto shot : ThePlayer->Shots)
	{
		if (CirclesIntersect(shot))
		{
			shot->Enabled = false;
			GiveScore();
			return true;
		}
	}

	return false;
}

bool UFO::CheckReachedSide()
{
	if (X() < -WindowWidth)
	{
		return true;
	}
	if (X() > WindowWidth)
	{
		return true;
	}

	return false;
}