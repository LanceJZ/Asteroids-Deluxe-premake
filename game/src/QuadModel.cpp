#include "QuadModel.h"

void QuadModel::LoadModel(Model model)
{
	TheModel = model;
}

void QuadModel::Update(float deltaTime)
{
	if (Enabled)
	{
		Entity::Update(deltaTime);
		TheModel.transform = MatrixRotateZ(Rotation);	// Rotate 3D model
	}
}

void QuadModel::Draw()
{
	if (Enabled)
	{
		DrawModel(TheModel, Position, Scale, RAYWHITE);	// Draw 3D model
		Entity::Draw();
	}
}