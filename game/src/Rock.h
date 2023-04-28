#pragma once
#include "LineModel.h"
#include "Player.h"
#include "UFO.h"
#include "Exploder.h"

class Rock : public LineModel
{
public:
	enum RockSize
	{
		Small,
		Medium,
		Large
	};

	RockSize Size = Large;
	Exploder TheExploder;

	Rock(float windowWidth, float windowHeight, Player* player, UFO* ufo, Color TheColor);
	~Rock();

	void LoadSound(Sound exp);
	void SetDotModel(vector<Vector3> dotModel);
	bool Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn(Vector3 pos, float speed, RockSize size);


private:
	Player* ThePlayer = nullptr;
	UFO* TheUFO = nullptr;

	void GiveScore();
	bool CheckCollision();
	vector<Vector3> DotModel;
};

