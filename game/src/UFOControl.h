#pragma once
#include "raylib.h"
#include "Common.h"
#include "Player.h"
#include "UFO.h"
#include "Timer.h"

class UFOControl : public Common
{
public:

	virtual void LoadModel(string ship, vector<Vector3> dotModel);
	virtual void Update(float deltaTime);
	void LoadSound(Sound exp, Sound big, Sound small, Sound fire);
	virtual void Draw();
	bool Initialize();

	void NewGame();

	UFOControl(float playScreenW, float playScreenH, Player* player, CrossCom* crossCom, Color color);
	UFO* TheUFO;

private:
	int SpawnCount = {0};

	Timer* SpawnTimer;
	Player* ThePlayer;
	CrossCom* Comm;

	void SpawnUFO();
	void ResetTimer();
};

