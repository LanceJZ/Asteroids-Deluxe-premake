#pragma once
#include "LineModel.h"
#include "Player.h"
#include "Shot.h"
#include "Exploder.h"
#include "Timer.h"
#include "CrossCom.h"
#include "RockData.h"

class UFO : public LineModel
{
public:
	enum Size
	{
		Small,
		Large
	};

	Size size = Large;

	Shot TheShot;
	Exploder TheExploder;
	std::vector<RockData*> Rocks;

	UFO();
	~UFO();

	bool Initialize(float windowWidth, float windowHeight, Player* player,
		CrossCom* crossCom, Color TheColor);
	void LoadModel(string ship, vector<Vector3> dotModel);
	void LoadSound(Sound exp, Sound big, Sound small, Sound fire);
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn(Vector3 pos, Vector3 vel);
	void Collision();


private:
	Timer FireTimer;
	Timer VectorTimer;

	Player* ThePlayer = nullptr;
	CrossCom* Comm = nullptr;

	void GiveScore();
	void ResetFireTimer();
	void ResetVectorTimer();
	void ChangeVector();
	void FireShot();
	float AimedShot();
	float AimedShotAtWGroup();
	float AimedShotAtRock();
	bool CheckCollision();
	bool CheckReachedSide();
};

