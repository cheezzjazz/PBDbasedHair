#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Vec3.h"

using namespace std;

class Vertex
{
public:
	Vec3<float> position;
	Vec3<float> normal;
	vector<int> neighborFaces;
};

class Face
{
public:
	int vertexIdx[3];
	Vec3<float> v0, v1, v2;
	Vec3<float> normal;
};

class MeshLoader
{
public:
	vector<Vertex> vertexArray;
	vector<Face> faceArray;

	bool LoadMeshfile(string filename);
	void ComputeFaceNormal();
	void FindNeighborFaces();
	void ComputeVertexNormal();

};