#pragma once
#include <vector>
#include "raylib.h"
#include "Common.h"
#include "Rock.h"
#include "CrossCom.h"
using namespace std;

class RockControl : public Common
{
public:
	RockControl();
	~RockControl();

	bool RockCountUnderFour = false;
	bool Debug = false;
	vector<Rock*> Rocks;

	bool Initialize(float screenWidth, float screenHeight, Player* player, UFO* ufo,
		CrossCom* crosscom, Color TheColor);
	virtual void LoadModel(string modelOne, string modelTwo, string modelThree,
		string modelFour, vector<Vector3> dotModel);
	void LoadSound(Sound exp);
	virtual void Update(float deltaTime);
	virtual void Draw();
	void NewGame(void);
	void NewWave(void);
	void RockHit(Rock* rockHit);


private:
	bool NoRocks = false;
	int NewRockCount = { 4 };
	Color TheColor = WHITE;
	LineModel RockModels[4];
	vector<Vector3> DotModel;
	Sound ExplodeSound = { 0 };

	void SpawnRocks(Vector3 pos, int count, Rock::RockSize size);
	Player* ThePlayer = nullptr;
	UFO* TheUFO = nullptr;
	CrossCom* Comm = nullptr;
};

