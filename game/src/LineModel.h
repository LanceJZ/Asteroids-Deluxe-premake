#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include "rlgl.h"

class LineModel : public Entity
{
public:
	Color ModelColor = WHITE;
	float Alpha = 255;

	LineModel();
	virtual ~LineModel();

	void Load();
	void LoadModel(string fileName);

	virtual void Update(float deltaTime);
	virtual void Draw();


	vector<Vector3> GetModel();
	vector<Vector3> LinePoints;
	void SetModel(vector<Vector3> lines);

private:
	vector<Vector3> ConvertStringToVector(string linesString);
};