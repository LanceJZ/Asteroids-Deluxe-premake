#pragma once
#include "Entity.h"

class QuadModel : public Entity
{
public:
	Model TheModel;

	virtual void LoadModel(Model model);
	virtual void Update(float deltaTime);
	virtual void Draw();

private:
};

