#include "Exploder.h"

Exploder::Exploder(vector<Vector3> dotModel, Color color)
{
	Exploder::DotModel.SetModel(dotModel);
	Exploder::TheColor = color;
}

void Exploder::Update(float deltaTime)
{
	for (auto dot : Dots)
	{
		dot->Update(deltaTime);
	}
}

void Exploder::Draw()
{
	for (auto dot : Dots)
	{
		dot->Draw();
	}
}

bool Exploder::Initialise()
{
	return false;
}

void Exploder::Spawn(Vector3 position, int count, float radius)
{
	for (int i = 0; i < count; i++)
	{
		bool spawnDot = true;
		int dot = (int)Dots.size();

		for (int c = 0; c < dot; c++)
		{
			if (!Dots[c]->Enabled)
			{
				dot = c;
				spawnDot = false;
				break;
			}
		}

		if (spawnDot)
		{
			Dots.push_back(new Dot());
			Dots[dot]->Initialise();
			Dots[dot]->SetModel(DotModel.GetModel());
			Dots[dot]->ModelColor = TheColor;
		}

		Dots[dot]->Spawn(position, count / 10.0f, radius / 2.0f);
	}
}

void Exploder::Clear()
{
	for (auto dot : Dots)
	{
		dot->Enabled = false;
	}
}
