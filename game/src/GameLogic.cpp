#include "GameLogic.h"

GameLogic::GameLogic()
{
	float per = 42.75f;
	float playScreenW = GetScreenWidth() / per;
	float playScreenH = GetScreenHeight() / per;

	rock1.WindowWidth = playScreenW;
	rock1.WindowHeight = playScreenH;
	rock2.WindowWidth = playScreenW;
	rock2.WindowHeight = playScreenH;
	rock3.WindowWidth = playScreenW;
	rock3.WindowHeight = playScreenH;
	rock4.WindowWidth = playScreenW;
	rock4.WindowHeight = playScreenH;
}

GameLogic::~GameLogic()
{
}

void GameLogic::Load()
{
	rock1.LoadModel("Models/RockOne.vec");
	rock2.LoadModel("Models/RockTwo.vec");
	rock3.LoadModel("Models/RockThree.vec");
	rock4.LoadModel("Models/RockFour.vec");
}

bool GameLogic::BeginRun()
{
	rock2.Position = { 2.5f, 2.5f, 0 };
	rock3.Position = { 2.5f, -2.5f, 0 };
	rock4.Position = { -2.5f, -2.5f, 0 };

	rock1.AddChild(&rock2);
	rock2.AddChild(&rock4);
	rock4.AddChild(&rock3);

	rock1.Velocity.x = 0.5f;
	rock1.Velocity.y = 0.75f;
	rock1.RotationVelocity = 0.1f;
	rock2.RotationVelocity = 0.5f;
	rock3.RotationVelocity = -0.25f;

	return false;
}

void GameLogic::Input()
{

}

void GameLogic::Update(float deltaTime)
{
	rock1.Update(deltaTime);
	rock2.Update(deltaTime);
	rock3.Update(deltaTime);
	rock4.Update(deltaTime);

	rock1.CheckScreenEdge();
}

void GameLogic::Draw3D()
{
	rock1.Draw();
	rock2.Draw();
	rock3.Draw();
	rock4.Draw();
}

void GameLogic::Draw2D()
{

}
