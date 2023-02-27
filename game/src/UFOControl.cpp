#include "UFOControl.h"
#include "raymath.h"

UFOControl::UFOControl(float playScreenW, float playScreenH, Player* player, CrossCom* crossCom, Color color)
{
	GameScreenWidth = playScreenW;
	GameScreenHeight = playScreenH;
	UFOControl::ThePlayer = player;

	TheUFO = new UFO(playScreenW, playScreenH, player, crossCom, color);

	SpawnTimer = new Timer();
}

bool UFOControl::Initialize()
{
	TheUFO->Initialise();
	ResetTimer();

	return false;
}

void UFOControl::LoadModel(string ship, vector<Vector3> dotModel)
{
	TheUFO->LoadModel(ship, dotModel);
}

void UFOControl::LoadSound(Sound exp, Sound big, Sound small, Sound fire)
{
	TheUFO->LoadSound(exp, big, small, fire);
}

void UFOControl::Update(float deltaTime)
{
	TheUFO->Update(deltaTime);
	SpawnTimer->Update(deltaTime);

	if (SpawnTimer->Elapsed() && !TheUFO->Enabled)
	{
		SpawnUFO();
	}
	else if (TheUFO->Enabled)
	{
		ResetTimer();
	}
}

void UFOControl::Draw()
{
	TheUFO->Draw();
}

void UFOControl::NewGame()
{
	ResetTimer();
	SpawnCount = 0;
	TheUFO->Enabled = false;
	TheUFO->TheShot->Enabled = false;
}

void UFOControl::SpawnUFO()
{
	ResetTimer();

	if (!ThePlayer->Enabled && !ThePlayer->GameOver)
		return;

	SpawnCount++;
	float spawnPercent = (powf(0.915f, (float)SpawnCount / (float)(ThePlayer->Wave + 1)) * 100);

	if (GetRandomFloat(0, 99) < spawnPercent - ThePlayer->Score / 500)
	{
		TheUFO->size = UFO::Large;
		TheUFO->Scale = 1;
		TheUFO->MaxSpeed = 5.0f;
		TheUFO->Radius = 0.75f;
	}
	else
	{
		TheUFO->size = UFO::Small;
		TheUFO->Scale = 0.5f;
		TheUFO->MaxSpeed = 7.0f;
		TheUFO->Radius = 0.4f;
	}

	float speed = 0;
	float height = GetRandomFloat(GameScreenHeight / 4, GameScreenHeight / 1.25f);
	float side = 0;

	if (GetRandomFloat(0, 10) < 5)
	{
		speed = 5;
		side = -GameScreenWidth;
	}
	else
	{
		speed = -5;
		side = GameScreenWidth;
	}

	TheUFO->Spawn({ side, height, 0 }, { speed, 0, 0 });
}

void UFOControl::ResetTimer()
{
	float min = 10 - (ThePlayer->Wave * 0.1f);
	float max = 11 + (ThePlayer->Wave * 0.1f);
	SpawnTimer->Reset(GetRandomFloat(min, max));
}
