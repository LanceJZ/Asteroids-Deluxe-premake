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
	WedgeGroup* TheWedgeGroup;

	WedgeControl(float playScreenW, float playScreenH, Player* player, UFO* ufo, CrossCom* crosscom, Color color);
	virtual ~WedgeControl();

	bool Initialise();
	void LoadSound(Sound explode, Sound spawn);
	void LoadModel(string model);

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	Color TheColor = WHITE;
	Player* ThePlayer;
	Timer* SpawnTimer;
	CrossCom* Comm;
	Sound SpawnSound;

	void SpawnGroup();
};

