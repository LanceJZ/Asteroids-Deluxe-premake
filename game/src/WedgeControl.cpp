#include "WedgeControl.h"

WedgeControl::WedgeControl()
{
}

WedgeControl::~WedgeControl()
{
	UnloadSound(SpawnSound);
}

bool WedgeControl::Initialize(float playScreenW, float playScreenH, Player* player,
	UFO* ufo, CrossCom* crosscom, Color TheColor)
{
	GameScreenWidth = playScreenW;
	GameScreenHeight = playScreenH;
	WedgeControl::ThePlayer = player;
	WedgeControl::Comm = crosscom;
	WedgeControl::TheColor = TheColor;
	TheWedgeGroup.Initialize(playScreenW, playScreenH, player, ufo, Comm, TheColor);

	SpawnTimer.Reset(3);
	Ready = false;

	return false;
}

void WedgeControl::LoadSound(Sound explode, Sound spawn)
{
	WedgeControl::SpawnSound = spawn;
	TheWedgeGroup.LoadSound(explode);
}

void WedgeControl::LoadModel(string model)
{
	TheWedgeGroup.LoadModel(model);
}

void WedgeControl::Input()
{

}

void WedgeControl::Update(float deltaTime)
{
	TheWedgeGroup.Update(deltaTime);
	SpawnTimer.Update(deltaTime);

	if (!Comm->SpawnWedgeGroup)
	{
		SpawnTimer.Reset();
		return;
	}

	if (TheWedgeGroup.Enabled)
	{
		SpawnTimer.Reset();
		return;
	}

	bool resetTimer = false;

	for (auto &wedgePair : TheWedgeGroup.WedgePairs)
	{
		for (auto &wedge : wedgePair.Wedges)
		{
			if (wedge.Enabled)
			{
				resetTimer = true;
			}
		}
	}

	if (resetTimer)
	{
		SpawnTimer.Reset();
		return;
	}

	if (SpawnTimer.Elapsed())
	{
		SpawnTimer.Reset();

		if (Ready)
		{
			SpawnGroup();
		}
		else if (!Ready)
		{
			Ready = true;
		}
	}
}

void WedgeControl::Draw()
{
	TheWedgeGroup.Draw();
}

void WedgeControl::SpawnGroup()
{
	if (!ThePlayer->Enabled)
		return;

	if (!ThePlayer->GameOver)
		PlaySound(SpawnSound);

	Ready = false;

	TheWedgeGroup.Spawn(GetRandomVelocity(1.5f));
}
