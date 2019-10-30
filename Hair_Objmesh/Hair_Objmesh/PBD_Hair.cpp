#include "PBD_Hair.h"
#include "GL\glut.h"

PBD_Hairstrand::PBD_Hairstrand()
{
}

PBD_Hairstrand::PBD_Hairstrand(float length, int count)
{
	m_Length = length;
	m_Count = count;
	m_Pos.resize(m_Count);
	m_NewPos.resize(m_Count);
	m_Vel.resize(m_Count);
	m_Mass.resize(m_Count);
	m_InitPos = Vec3<float>(0.0f, 0.0f, 0.0f);

	Init();
	ComputeRestLength();
}

PBD_Hairstrand::PBD_Hairstrand(Vec3<float> position, float length, int count)
{
	m_Length = length;
	m_Count = count;
	m_Pos.resize(m_Count);
	m_NewPos.resize(m_Count);
	m_Vel.resize(m_Count);
	m_Mass.resize(m_Count);
	m_InitPos = position;

	Init();
	ComputeRestLength();
}

PBD_Hairstrand::~PBD_Hairstrand()
{
}

void PBD_Hairstrand::Init()
{
	for (int i = 0; i < m_Count; i++)
	{
		m_Mass[i] = 1.0f;
		m_Pos[i].Set(m_InitPos.GetX() + (m_Length * i / (float)m_Count), m_InitPos.GetY() + 0.0f, m_InitPos.GetZ() + 0.0f);
	}
}

void PBD_Hairstrand::Simulation(float dt)
{
	ApplyExternalForces(dt);
	UpdateStructuralSprings();
	UpdateBendSprings();
	Integrate(dt);
}

void PBD_Hairstrand::Integrate(float dt)
{
	for (int i = 1; i < m_Count; i++)
	{
		m_Vel[i] = (m_NewPos[i] - m_Pos[i]) / dt;
		m_Pos[i] = m_NewPos[i];
	}
}

void PBD_Hairstrand::ComputeRestLength()
{
	m_Strt_RestLength = m_Length / (float)m_Count;
	m_Bend_RestLength = m_Strt_RestLength * 2.0f;
}

//void PBD_Hairstrand::ComputeDihedralAngle()
//{
//}

void PBD_Hairstrand::UpdateStructuralSprings()
{
	for (int i = 0; i < m_Count - 1; i++)
	{
		int index0 = i;
		int index1 = i + 1;
		SolveDistanceConstraint(index0, index1, m_Strt_RestLength);
	}
}

void PBD_Hairstrand::UpdateBendSprings()
{
	for (int i = 0; i < m_Count - 2; i++)
	{
		int index0 = i;
		int index1 = i + 2;
		SolveDistanceConstraint(index0, index1, m_Bend_RestLength);
	}
}

void PBD_Hairstrand::SolveDistanceConstraint(int index0, int index1, float restLength)
{
	float C_p1p2 = (m_NewPos[index0] - m_NewPos[index1]).Length() - restLength;
	Vec3<float> delp1 = (m_NewPos[index0] - m_NewPos[index1]);
	Vec3<float> delp2 = (m_NewPos[index0] - m_NewPos[index1]);
	delp1.Normalize();
	delp2.Normalize();
	delp1 *= -m_Mass[index0] / (m_Mass[index0] + m_Mass[index1]) * C_p1p2;
	delp2 *= m_Mass[index1] / (m_Mass[index0] + m_Mass[index1]) * C_p1p2;
	m_NewPos[index0] += delp1;
	m_NewPos[index1] += delp2;
}

void PBD_Hairstrand::ApplyExternalForces(float dt)
{
	Vec3<float> gravity(0.0f, -9.8f, 0.0f);
	float damping = 0.99f;
	for (int i = 0; i < m_Count; i++)
	{
		m_Vel[i] += gravity * m_Mass[i] * dt;
		m_Vel[i] *= damping;
		m_NewPos[i] = m_Pos[i] + m_Vel[i] * dt;
	}
}

void PBD_Hairstrand::DrawPoint()
{
	glPointSize(2.0f);
	for (int i = 0; i < m_Count; i++)
	{
		auto p = getPos(i);
		glBegin(GL_POINTS);
		glVertex3f(p.GetX(), p.GetY(), p.GetZ());
		glEnd();
	}
}

void PBD_Hairstrand::DrawLine()
{
	for (int i = 0; i < m_Count-1; i++)
	{
		auto p0 = getPos(i);
		auto p1 = getPos(i + 1);
		glBegin(GL_LINES);
		glVertex3f(p0.GetX(), p0.GetY(), p0.GetZ());
		glVertex3f(p1.GetX(), p1.GetY(), p1.GetZ());
		glEnd();
	}
}

void PBD_Hairstrand::Draw()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	DrawPoint();
	glColor3f(0.802f, 0.515f, 0.332f);
	DrawLine();
}
