#pragma once
#include "raylib.h"
#include "LineModel.h"

class LineModelManager
{
public:
	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Load();
	void LoadModel(string model);

	LineModelManager();
	virtual ~LineModelManager();

private:

};

