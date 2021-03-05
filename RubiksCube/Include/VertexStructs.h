#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

// Helper struct to store and handle different attributes more elegantly
struct Vertex
{
public:
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texCoord;
	glm::vec3 m_tangent;
	glm::vec3 m_biTangent;
};


struct GeometryData
{
public:
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_normals;
	// Color is only present here as we're only assigning it through code so far
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec3> m_tangents;
	std::vector<glm::vec3> m_biTangents;
	std::vector<glm::vec2> m_texCoords;
	std::vector<uint32_t> m_indices;
};

template<typename Type>
struct CVec
{
public:
	Type x;
	Type y;
	Type z;
};

template<typename Type>
struct CVec2
{
public:
	CVec2(Type xt, Type yt) : x{xt}, y{yt} {}
	Type x;
	Type y;
};