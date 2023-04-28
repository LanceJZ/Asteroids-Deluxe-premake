#pragma once
#include "raylib.h"
#include "Common.h"
#include "Player.h"
#include "UFO.h"
#include "Timer.h"

class UFOControl : public Common
{
public:
	UFOControl();
	~UFOControl();

	virtual void LoadModel(string ship, vector<Vector3> dotModel);
	virtual void Update(float deltaTime);
	void LoadSound(Sound exp, Sound big, Sound small, Sound fire);
	virtual void Draw();
	bool Initialize(float playScreenW, float playScreenH, Player* player,
		CrossCom* crossCom, Color TheColor);

	void NewGame();

	UFO TheUFO;

private:
	int SpawnCount = {0};

	Player* ThePlayer = nullptr;
	CrossCom* Comm = nullptr;
	Timer SpawnTimer;

	void SpawnUFO();
	void ResetTimer();
};

