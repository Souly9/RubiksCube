#include "stdafx.h"
#include "Cubie.h"
std::vector<glm::vec3> Cubie::m_colorVals{RUBIX_RED, RUBIX_YELLOW, RUBIX_WHITE, RUBIX_BLUE, RUBIX_ORANGE, RUBIX_GREEN};

Cubie::Cubie(const Mesh3D& cubeMesh, glm::vec3 center)
	: Mesh3D{cubeMesh}, m_colors{RubixCubeEnums::Color::None}
	  
{
	SetCenter(center);
	m_translation = center;
}

void Cubie::AddColor(std::vector<RubixCubeEnums::Color> colors)
{
	// Iterates through all assigned colors and assigns the vertices the right color
	// Cube faces are the same order as the rubix faces
	using namespace RubixCubeEnums;
	m_colors = colors;
	
	m_data->m_colors.reserve(RUBIX_VERTICES_PER_SIDE * 6);
	auto color = 0;
	while(color < 6)
	{
		for (int i = 0; i < RUBIX_VERTICES_PER_SIDE; ++i)
		{
			m_data->m_colors.emplace_back(m_colorVals.at(color));
		}
		color++;
	}
}

auto Cubie::GetColors() const -> const std::vector<RubixCubeEnums::Color>
{
	return m_colors;
}

CVec<int> Cubie::GetCenter() const
{
	return m_center;
}

void Cubie::SetCenter(glm::vec3 cen)
{
	const int x = round(cen.x), y = round(cen.y), z = round(cen.z);
	SetCenter(CVec<int>{x, y, z});
}

void Cubie::SetCenter(CVec<int> cen)
{
	m_translation = glm::vec3(cen.x, cen.y, cen.z);
	m_center = cen;
}
