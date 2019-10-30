#pragma once
#include "Vec3.h"
#include <vector>

using namespace std;

class PBD_Hairstrand
{
public:
	float m_Length;		//Hair length
	int m_Count;		//Hairstrand Particle counts
	Vec3<float> m_InitPos;	//Init Position
	// Vertex
	vector<Vec3<float>> m_Pos;
	vector<Vec3<float>> m_NewPos;
	vector<Vec3<float>> m_Vel;
	vector<float> m_Mass;
	// Hair
	vector<float> m_DihedralAngle;	//�̸鰢
public:
	float m_Strt_RestLength;	//structural 
	float m_Bend_RestLength;	//bending
public:
	PBD_Hairstrand();
	PBD_Hairstrand(float length, int count);
	PBD_Hairstrand(Vec3<float> position, float length, int count);
	~PBD_Hairstrand();
public:
	inline Vec3<float> getPos(int i) { return m_Pos[i]; }
	void Init();
	void Simulation(float dt);
	void Integrate(float dt);
	void ComputeRestLength();
	//void ComputeDihedralAngle();
	void UpdateStructuralSprings();
	void UpdateBendSprings();
	void SolveDistanceConstraint(int index0, int index1, float restLength);
	//void SolveDihedralConstraint(int index0, int index1, float restAngle);
	void ApplyExternalForces(float dt);
	void DrawPoint();
	void Draw();
};
