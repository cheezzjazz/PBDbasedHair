#include "meshLoader.h"

bool MeshLoader::LoadMeshfile(string filename)
{
	//read file
	ifstream readFile(filename);
	if (!readFile.is_open())
	{
		cout << "ERROR : Failed to open file " << filename << "!" << endl;
		return false;
	}
	
	string line;
	char delimiter = ' ';
	int idx = 0;
	while (getline(readFile, line))
	{
		//cout << line << endl;
		string header, context;
		stringstream sline(line);
		getline(sline, header, ' ');
		if (header == "v")
		{
		//	cout << "v : header - " << header<<endl;
			Vertex v;
			sline >> context;
			v.position.SetX(stof(context));// cout << " next - x:" << stof(context) << endl;
			sline >> context;
			v.position.SetY(stof(context));// cout << " next - y:" << stof(context) << endl;
			sline >> context;
			v.position.SetZ(stof(context));// cout << " next - z:" << stof(context) << endl;

			vertexArray.push_back(v);
		}
		else if (header == "vt")
		{
			//	cout << "vt : header - " << header << endl;
		}
		else if (header == "vn")
		{
			/*if (vertexArray.size() != 0 && vertexArray.size() > idx)
			{
				cout << "vn : header - " << header << endl;
				Vertex v = vertexArray.at(idx++);
				sline >> context;
				v.normal.SetX(stof(context)); cout << " next - x:" << stof(context) << endl;
				sline >> context;
				v.normal.SetY(stof(context)); cout << " next - y:" << stof(context) << endl;
				sline >> context;
				v.normal.SetZ(stof(context)); cout << " next - z:" << stof(context) << endl;
			}
			*/
		}
		else if (header == "f")
		{
			//cout << "f : header - " << header << endl;

			Face f;
			sline >> context;
			f.vertexIdx[0] = stoi(context)-1;// cout << " next - x:" << stoi(context) << endl;
			sline >> context;
			f.vertexIdx[1] = stoi(context)-1;// cout << " next - y:" << stoi(context) << endl;
			sline >> context;
			f.vertexIdx[2] = stoi(context)-1;// cout << " next - z:" << stoi(context) << endl;

			faceArray.push_back(f);
		}
	}

	readFile.close();
	return true;
}

void MeshLoader::ComputeFaceNormal()
{
	for (auto &f : faceArray)
	{
		f.v0 = vertexArray[f.vertexIdx[0]].position;
		f.v1 = vertexArray[f.vertexIdx[1]].position;
		f.v2 = vertexArray[f.vertexIdx[2]].position;
		
		Vec3<float> va = f.v1 - f.v0;
		Vec3<float> vb = f.v2 - f.v0;
		Vec3<float> vc = va.Cross(vb);
		vc.Normalize();
		f.normal = vc;
	}
}

void MeshLoader::FindNeighborFaces()
{
	for (int i = 0; i < vertexArray.size(); i++)
	{
		for (int j = 0 ; j < faceArray.size(); j++)
		{
			Face &f = faceArray[j];
			if (f.vertexIdx[0] == i || f.vertexIdx[1] == i || f.vertexIdx[2] == i)
			{
				vertexArray[i].neighborFaces.push_back(j);
			}
		}
	}
}

void MeshLoader::ComputeVertexNormal()
{
	for (auto &v : vertexArray)
	{
		for (int i : v.neighborFaces)
		{
			v.normal += faceArray[i].normal;
		}
		v.normal /= v.neighborFaces.size();
	}
}

