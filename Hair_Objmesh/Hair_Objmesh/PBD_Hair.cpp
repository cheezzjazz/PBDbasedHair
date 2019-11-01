#include "PBD_Hair.h"

PBD_Hair::PBD_Hair()
{
}

PBD_Hair::PBD_Hair(float length, int count)
{
	PBD_Hairstrand *h = new PBD_Hairstrand(length, count);
	myHair.push_back(h);
	cout << myHair.size() << endl;
}

PBD_Hair::PBD_Hair(float length, int count, float radius)
{
	float R = radius;
	for (int i = -R; i <= R; i++)
	{
		for (int j = -R; j <= R; j++)
		{
			if ((float)i*(float)i + (float)j*(float)j == R*R)
			{
				Vec3<float> InitPos(i, 0.0f, j);
				PBD_Hairstrand *h = new PBD_Hairstrand(InitPos, length, count);
				myHair.push_back(h);
			}
		}
	}
	cout << myHair.size() << endl;
}

PBD_Hair::PBD_Hair(float length, int count, int line_length)
{
	Vec3<float> InitPos(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < line_length; i++)
	{
		InitPos.SetX(InitPos.GetX() + 1.0f);
		PBD_Hairstrand *h = new PBD_Hairstrand(InitPos, length, count);
		myHair.push_back(h);
	}
	cout << myHair.size() << endl;

}

PBD_Hair::PBD_Hair(string meshFilename, float length, int count)
{
	myMesh = new MeshLoader();

	if (myMesh->LoadMeshfile(meshFilename))
	{
		for (auto &v : myMesh->vertexArray)
		{
			Vec3<float> InitPos = v.position;
			if (v.position.GetY() < 0.5f)
				continue;
			PBD_Hairstrand *h = new PBD_Hairstrand(InitPos, length, count);
			myHair.push_back(h);
		}
		cout << myHair.size() << endl;
	}
}

PBD_Hair::~PBD_Hair()
{
}

void PBD_Hair::Init()
{
	for (auto &hairstrand : myHair)
	{
		hairstrand->Init();
	}
}

void PBD_Hair::Simulation(float dt)
{
	for (auto &hairstrand : myHair)
	{
		hairstrand->Simulation(dt);
	}
}

void PBD_Hair::Draw()
{
	if (myMesh != NULL)
	{
		myMesh->RenderMesh();
	}
	for (auto &hairstrand : myHair)
	{
		hairstrand->Draw();
	}
	
}

void PBD_Hair::ApplyWindtoHair(Vec3<float> wind)
{
	for (auto &hairstrand : myHair)
	{
		hairstrand->ApplyWind(wind);
	}
}
