#include "Line.h"
#include "raymath.h"

Line::Line(Color TheColor)
{
	TheTimer = new Timer();
	Enabled = false;
	Line::TheColor = TheColor;
}

Line::~Line()
{
}
void Line::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	TheTimer->Update(deltaTime);

	PosStart = Vector3Add(StartPos, Position);
	PosEnd = Vector3Add(EndPos, Position);

	if (TheTimer->Elapsed())
	{
		Enabled = false;
	}
}

void Line::Draw()
{
	if (Enabled)
		DrawLine3D(PosStart, PosEnd, TheColor);
}

void Line::Load()
{

}

void Line::LoadModel(Model model, Model shotmodel)
{

}

void Line::Spawn(Vector3 pos)
{
	float size = 0.75f;
	Position = pos;
	Velocity = { GetRandomFloat(-size, size), GetRandomFloat(-size, size) , 0 };
	StartPos = {GetRandomFloat(-size, size), GetRandomFloat(-size, size) , 0};
	EndPos = { GetRandomFloat(-size, size), GetRandomFloat(-size, size) , 0 };

	Enabled = true;
	TheTimer->Reset(GetRandomFloat(0.5f, 2.666f));
}

void Line::Clear()
{
	Enabled = false;
}