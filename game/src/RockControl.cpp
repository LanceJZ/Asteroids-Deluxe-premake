 #include "RockControl.h"
#include <vector>

RockControl::RockControl()
{
}

RockControl::~RockControl()
{
	UnloadSound(ExplodeSound);

	for (int i = 0; i < Rocks.size(); i++)
	{
		delete Rocks[i];
	}

	Rocks.clear();
}

bool RockControl::Initialize(float screenWidth, float screenHeight, Player* player,
	UFO* ufo, CrossCom* crosscom, Color TheColor)
{
	GameScreenWidth = screenWidth;
	GameScreenHeight = screenHeight;
	RockControl::ThePlayer = player;
	RockControl::TheUFO = ufo;
	RockControl::Comm = crosscom;
	RockControl::TheColor = TheColor;

	return false;
}

void RockControl::LoadModel(string modelOne, string modelTwo, string modelThree,
	string modelFour, vector<Vector3> dotModel)
{
	RockModels[0].LoadModel(modelOne);
	RockModels[1].LoadModel(modelTwo);
	RockModels[2].LoadModel(modelThree);
	RockModels[3].LoadModel(modelFour);

	RockControl::DotModel = dotModel;
}

void RockControl::LoadSound(Sound exp)
{
	ExplodeSound = exp;
}

void RockControl::Update(float deltaTime)
{
	bool rockHit = false;
	Rock* rockWasHit = NULL;

	for (auto rock : Rocks)
	{
		if (rock->BeenHit)
		{
			rockHit = true;
			rockWasHit = rock;
			rockWasHit->BeenHit = false;
		}

		rock->Update(deltaTime);
	}

	for (int i = 0; i < Rocks.size(); i++)
	{
		TheUFO->Rocks[i]->Update(Rocks[i]->Position, Rocks[i]->Enabled);
	}

	if (rockHit)
	{
		RockHit(rockWasHit);
	}

	if (NoRocks)
	{
		if (!TheUFO->Enabled)
		{
			NewWave();
			NoRocks = false;
		}
	}
}

void RockControl::Draw(void)
{
	for (auto rock : Rocks)
	{
		rock->Draw();
	}
}

void RockControl::NewGame(void)
{
	for (auto rock : Rocks)
	{
		rock->Enabled = false;
	}

	NewRockCount = 4;
	NewWave();
}

void RockControl::NewWave(void)
{
	Comm->NewWave = true;
	Comm->SpawnWedgeGroup = false;
	ThePlayer->Wave++;
	SpawnRocks({ 0, 0, 0 }, NewRockCount, Rock::Large);


	if (NewRockCount < 12)
		NewRockCount++;
}

void RockControl::RockHit(Rock* rockHit)
{
	rockHit->TheExploder.Spawn(rockHit->Position, 15, rockHit->Radius);

	switch (rockHit->Size)
	{
	case Rock::Large:
		SpawnRocks(rockHit->Position, 2, Rock::Medium);
		break;

	case Rock::Medium:
		SpawnRocks(rockHit->Position, 2, Rock::Small);
		break;

	case Rock::Small:
		bool rocksGone = true;

		for (auto rock: Rocks)
		{
			if (rock->Enabled)
				rocksGone = false;
		}

		if (rocksGone)
			NoRocks = true;
;
		break;
	}

	int activeRockCount = 0;

	for (auto rock : Rocks)
	{
		if (rock->Enabled)
		{
			activeRockCount++;
		}
	}

	if (activeRockCount < (NewRockCount - 1))
	{
		Comm->SpawnWedgeGroup = true;
	}
	else
	{
		Comm->SpawnWedgeGroup = false;
	}
}

void RockControl::SpawnRocks(Vector3 pos, int count, Rock::RockSize size)
{
	for (int rock = 0; rock < count; rock++)
	{
		bool spawnnewrock = true;
		int rockN = (int)Rocks.size();
		float maxSpeed = 10.666f;

		for (int rockcheck = 0; rockcheck < rockN; rockcheck++)
		{
			if (!Rocks[rockcheck]->Enabled)
			{
				spawnnewrock = false;
				rockN = rockcheck;
				break;
			}
		}

		if (spawnnewrock)
		{
			Rocks.push_back(new Rock(GameScreenWidth, GameScreenHeight, ThePlayer, TheUFO, TheColor));
			Rocks[rockN]->SetModel(RockModels[GetRandomValue(0, 3)].GetModel());
			Rocks[rockN]->SetDotModel(DotModel);
			Rocks[rockN]->LoadSound(ExplodeSound);
			Rocks[rockN]->Initialize();
			Rocks[rockN]->Debug = Debug;

			TheUFO->Rocks.push_back(new RockData());
		}

		switch (size)
		{
			float speed;

		case Rock::Large:
			speed = GetRandomFloat(maxSpeed / 10, maxSpeed / 3);
			Rocks[rockN]->Spawn({ GameScreenWidth, GetRandomScreenY(), 0 }, speed, size);
			break;

		case Rock::Medium:
			speed = GetRandomFloat(maxSpeed / 10, maxSpeed / 2);
			Rocks[rockN]->Spawn(pos, speed, size);
			break;

		case Rock::Small:
			speed = GetRandomFloat(maxSpeed / 10, maxSpeed);
			Rocks[rockN]->Spawn(pos, speed, size);
			break;
		}

		TheUFO->Rocks[rockN]->SetVelocity(Rocks[rockN]->Velocity);
	}
}