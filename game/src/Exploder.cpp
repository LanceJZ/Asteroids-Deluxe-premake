#include "Exploder.h"

Exploder::Exploder()
{
}

Exploder::~Exploder()
{
	for (int i = 0; i < Dots.size(); i++)
	{
		delete Dots[i];
	}

	Dots.clear();
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

bool Exploder::Initialize(vector<Vector3> dotModel, Color TheColor)
{
	Exploder::DotModel.SetModel(dotModel);
	Exploder::TheColor = TheColor;

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
			Dots[dot]->Initialize();
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
