#pragma once
#include "raylib.h"
#include "Entity.h"
#include "Timer.h"

class Line : public Entity
{
public:
	Vector3 StartPos = { 0 };
	Vector3 EndPos = { 0 };
	Vector3 PosStart = { 0 };
	Vector3 PosEnd = { 0 };

	Line(Color TheColor);
	virtual ~Line();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Load();
	void LoadModel(Model model, Model shotmodel);
	void Spawn(Vector3 pos);
	void Clear();

private:

	Color TheColor = WHITE;
	Timer* TheTimer;
};

