#pragma once

class Mesh3D;
class Object;

/*
 * Mainly storage class for cubie specific attributes
 */
class Cubie : public Mesh3D
{
	std::vector<RubixCubeEnums::Color> m_colors;
	CVec<int> m_center;
	static std::vector<glm::vec3> m_colorVals;
	Cubie& operator=(const Cubie& other) = delete;
public:
	Cubie(const Mesh3D& cubeMesh, glm::vec3 center);

	void AddColor(std::vector<RubixCubeEnums::Color> colors);
	auto GetColors() const -> const std::vector<RubixCubeEnums::Color>;
	CVec<int> GetCenter() const;
	void SetCenter(glm::vec3);
	void SetCenter(CVec<int>);
};
