#pragma once
#include <vector>
#include "raylib.h"
#include "Dot.h"

class Exploder
{
public:
	Exploder(vector<Vector3> dotModel, Color color);
	virtual void Update(float deltaTime);
	virtual void Draw();
	bool Initialise();

	void Spawn(Vector3 position, int count, float radius);
	void Clear();

private:
	Color TheColor = WHITE;

	vector<Dot*> Dots;
	LineModel DotModel;
};

