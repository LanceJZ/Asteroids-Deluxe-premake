#pragma once
#include "Wedge.h"
#include "Player.h"
#include "UFO.h"
#include "CrossCom.h"

class WedgePair : public Entity
{
public:
	bool GroupDocked = true;
	bool WedgeDocked = true;
	bool NewWave = false;

	Wedge* Wedges[2];

	WedgePair(float windowWidth, float windowHeight, Player* player, UFO* ufo, CrossCom* crossCom, Color color);
	virtual ~WedgePair();

	bool Initialize();
	void LoadSound(Sound explode);
	void LoadModel(string model);
	void LoadWedgeModel(vector<Vector3> model);
	void SetDebug();

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn();

private:
	int TheScore = 100;
	float Speed = 3.25f;
	float TurnSpeed = 0.25f;
	Color TheColor = WHITE;

	LineModel WedgeModel;
	Player* ThePlayer;
	UFO* TheUFO;
	CrossCom* Comm;

	bool CheckCollision();
	void Collision();
	void ChasePlayer();
	void ChaseUFO();
	void TurnOff();
	void LeavePlay();
};

