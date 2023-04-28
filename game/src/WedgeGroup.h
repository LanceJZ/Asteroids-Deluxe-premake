#pragma once
#include "WedgePair.h"
#include "Player.h"
#include "UFO.h"
#include "CrossCom.h"
#include "Entity.h"

class WedgeGroup : public Entity
{
public:
	bool WedgepairsDocked = true;
	bool NewWave = false;

	WedgePair WedgePairs[3];

	WedgeGroup();
	virtual ~WedgeGroup();

	bool Initialize(float windowWidth, float windowHeight, Player* player,
		UFO* ufo, CrossCom* crossCom, Color TheColor);
	void LoadSound(Sound explode);
	void LoadModel(string model);

	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn(Vector3 velocity);

private:
	int Score = 50;
	Color TheColor = WHITE;

	LineModel WedgeModel;
	Player* ThePlayer = nullptr;
	UFO* TheUFO = nullptr;
	CrossCom* Comm = nullptr;

	bool CheckCollision();
	void Collision();
	void Undock();
	void Reset();
};

