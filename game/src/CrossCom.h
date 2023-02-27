#pragma once
#include <vector>
#include "raylib.h"

class CrossCom
{
public:
	bool SpawnWedgeGroup = false;
	bool NewWave = false;
	bool WedgeGroupActive = false;
	Vector3 WedgeGroupPos = {0};

	CrossCom();
	virtual ~CrossCom();

private:
};