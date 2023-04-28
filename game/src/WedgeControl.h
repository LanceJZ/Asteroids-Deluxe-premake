#pragma once
#include "WedgeGroup.h"
#include "Player.h"
#include "UFO.h"
#include "Timer.h"
#include "Common.h"
#include "CrossCom.h"

class WedgeControl : public Common
{
public:
	bool Ready = false;
	WedgeGroup TheWedgeGroup;

	WedgeControl();
	virtual ~WedgeControl();

	bool Initialize(float playScreenW, float playScreenH, Player* player,
		UFO* ufo, CrossCom* crosscom, Color TheColor);
	void LoadSound(Sound explode, Sound spawn);
	void LoadModel(string model);

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Color TheColor = WHITE;
	Sound SpawnSound = { 0 };
	Timer SpawnTimer;

	Player* ThePlayer = nullptr;
	CrossCom* Comm = nullptr;

	void SpawnGroup();
};

