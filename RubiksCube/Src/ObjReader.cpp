#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>

Mesh3D ObjReader::readOBJ(const char* filename)
{
	Clear();
	std::ifstream stream;
	stream.open(filename);

	std::string line;

	Mesh3D mesh;
	while (getline(stream, line))
	{
		// Get the specifier of the line (v, vt...)
		std::vector<std::string> c = splitString(line, ' ');

		// Did we read all of it?
		if (c.size() == 0) continue;

		if (c[0].compare("v") == 0)
			processPosition(c);
		else if (c[0].compare("vt") == 0)
			processTexCoord(c);
		else if (c[0].compare("vn") == 0)
			processNormal(c);
		else if (c[0].compare("f") == 0)
			processFace(c, mesh);
	}
	mesh.AddVertices(m_temporaryVertices);
	return mesh;
}

std::vector<std::string> ObjReader::splitString(std::string& str, char delimiter) const
{
	std::vector<std::string> words;
	std::string word;
	std::stringstream stream(str);

	while (std::getline(stream, word, delimiter))
		words.push_back(word);

	return words;
}

void ObjReader::processPosition(std::vector<std::string>& c)
{
	m_temporaryPositions.push_back(
		glm::vec3(std::stof(c[1]), std::stof(c[2]), std::stof(c[3])));
}

void ObjReader::processTexCoord(std::vector<std::string>& c)
{
	m_temporaryTexCoords.push_back(
		glm::vec2(std::stof(c[1]), std::stof(c[2])));
}

void ObjReader::processNormal(std::vector<std::string>& c)
{
	m_temporaryNormals.push_back(
		glm::vec3(std::stof(c[1]), std::stof(c[2]), std::stof(c[3])));
}

void ObjReader::processFace(std::vector<std::string>& c, Mesh3D& readMesh)
{
	for (int i = 1; i < c.size(); ++i)
	{
		auto iter = m_vertexHashMap.find(c[i]);

		if (iter == m_vertexHashMap.end())
		{
			std::vector<std::string> index = splitString(c[i], '/');
			;

			m_vertexHashMap[c[i]] = m_temporaryVertices.size();
			m_temporaryVertices.push_back({
				m_temporaryPositions[std::stoi(index[0]) - 1], m_temporaryNormals[std::stoi(index[2]) - 1],
				m_temporaryTexCoords[std::stoi(index[1]) - 1]
				});
		}
		else
		{
			m_temporaryVertices.push_back(m_temporaryVertices.at(m_vertexHashMap.at(c[i])));
		}
	}
	if (c.size() == 5)
	{
		auto in1 = m_vertexHashMap[c[1]];
		auto in2 = m_vertexHashMap[c[2]];
		auto in3 = m_vertexHashMap[c[3]];
		auto in4 = m_vertexHashMap[c[4]];
		Vertex vert1 = m_temporaryVertices.at(in1);
		Vertex vert2 = m_temporaryVertices.at(in2);
		Vertex vert3 = m_temporaryVertices.at(in3);
		Vertex vert4 = m_temporaryVertices.at(in4);
		ComputeTangents(vert1, vert2, vert3);
		ComputeTangents(vert2, vert3, vert4);
		readMesh.AddIndex(in1);
		readMesh.AddIndex(in2);
		readMesh.AddIndex(in4);

		readMesh.AddIndex(in2);
		readMesh.AddIndex(in3);
		readMesh.AddIndex(in4);
		m_temporaryVertices.at(in1) = vert1;
		m_temporaryVertices.at(in2) = vert2;
		m_temporaryVertices.at(in3) = vert3;
		m_temporaryVertices.at(in4) = vert4;
		return;
	}
	auto in1 = m_vertexHashMap[c[1]];
	auto in2 = m_vertexHashMap[c[2]];
	auto in3 = m_vertexHashMap[c[3]];
	Vertex vert1 = m_temporaryVertices.at(in1);
	Vertex vert2 = m_temporaryVertices.at(in2);
	Vertex vert3 = m_temporaryVertices.at(in3);
	ComputeTangents(vert1, vert2, vert3);
	readMesh.AddIndex(in1);
	readMesh.AddIndex(in2);
	readMesh.AddIndex(in3);
	m_temporaryVertices.at(in1) = vert1;
	m_temporaryVertices.at(in2) = vert2;
	m_temporaryVertices.at(in3) = vert3;
}

void ObjReader::Clear()
{
	m_temporaryNormals.clear();
	m_temporaryPositions.clear();
	m_temporaryTexCoords.clear();
	m_temporaryVertices.clear();
	m_vertexHashMap.clear();
}

void ObjReader::ComputeTangents(Vertex& vert1, Vertex& vert2, Vertex& vert3)
{
	const auto edge1 = vert2.m_position - vert1.m_position;
	const auto edge2 = vert3.m_position - vert1.m_position;
	const auto deltaUV1 = vert2.m_texCoord - vert1.m_texCoord;
	const auto deltaUV2 = vert3.m_texCoord - vert1.m_texCoord;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	vert1.m_tangent.x = vert2.m_tangent.x = vert3.m_tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	vert1.m_tangent.y = vert2.m_tangent.y = vert3.m_tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	vert1.m_tangent.z = vert2.m_tangent.z = vert3.m_tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	vert1.m_biTangent.x = vert2.m_biTangent.x = vert3.m_biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x
	);
	vert1.m_biTangent.y = vert2.m_biTangent.y = vert3.m_biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y
	);
	vert1.m_biTangent.z = vert2.m_biTangent.z = vert3.m_biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z
	);
}
