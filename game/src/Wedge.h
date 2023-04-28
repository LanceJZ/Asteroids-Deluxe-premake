#pragma once
#include "LineModel.h"
#include "Player.h"
#include "UFO.h"
#include "CrossCom.h"

class Wedge : public LineModel
{
public:
	bool Docked = true;
	bool NewWave = false;

	Wedge();
	virtual ~Wedge();

	bool Initialize(float windowWidth, float windowHeight, Player* player,
		UFO* ufo, CrossCom* crossCom, Color TheColor);
	void LoadSound(Sound explode);

	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn();

private:
	int TheScore = 200;
	float Speed = 3.75f;
	float TurnSpeed = 0.5f;
	float RotateMagnitude = PI / 2;

	Player* ThePlayer = nullptr;
	UFO* TheUFO = nullptr;
	CrossCom* Comm = nullptr;

	bool CheckCollision();
	void Collision();
	void ChasePlayer();
	void ChaseUFO();
	void TurnOff();
	void LeavePlay();
	void Reset();
};

