#pragma once
#include <map>
#include "stdafx.h"

class Mesh3D;
struct Vertex;

// Simple .obj reader
class ObjReader
{
public:
	Mesh3D readOBJ(const char* filename);
	static void ComputeTangents(Vertex& vert1, Vertex& vert2, Vertex& vert3);
protected:
	std::map<std::string, uint32_t> m_vertexHashMap;
	std::vector<glm::vec3> m_temporaryPositions;
	std::vector<glm::vec2> m_temporaryTexCoords;
	std::vector<glm::vec3> m_temporaryNormals;
	std::vector<Vertex> m_temporaryVertices;
private:
	void processPosition(std::vector<std::string>& c);
	void processTexCoord(std::vector<std::string>& c);
	void processNormal(std::vector<std::string>& c);
	void processFace(std::vector<std::string>& c, Mesh3D& readMesh);
	void Clear();
	
	std::vector<std::string> splitString(std::string& str, char delimiter) const;
};
