#pragma once
#include "raylib.h"
#include "LineModel.h"
#include "Timer.h"

class Dot : public LineModel
{
public:
	bool Initialise();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Spawn(Vector3 position, float time, float size);
	void Clear();

private:
	Timer* TheTimer;
};

