#pragma once
#include "Entity.h"
#include "Timer.h"
#include "LineModel.h"

class Shot : public LineModel
{
public:
	Shot();

	bool Initialize(float windowWidth, float windowHeight);
	virtual void Update(float deltaTime);
	virtual void Draw();
	void Spawn(Vector3 pos, Vector3 vel, float timerAmount);

private:
	Timer LifeTimer;
};