#pragma once
#include "meshLoader.h"
#include "PBD_Hairstrand.h"

class PBD_Hair
{
public:
	MeshLoader *myMesh;
	vector<PBD_Hairstrand *> myHair;
public:
	PBD_Hair();
	PBD_Hair(float length, int count);
	PBD_Hair(float length, int count, float radius);
	PBD_Hair(float length, int count, int line_length);
	PBD_Hair(string meshFilename, float length, int count);
	~PBD_Hair();
public:
	void Init();
	void Simulation(float dt);
	void Draw();
	void ApplyWindtoHair(Vec3<float> wind);

};