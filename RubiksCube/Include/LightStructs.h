#pragma once

struct Light
{
public:
	Light(glm::vec4 pos, glm::vec4 color) :
		m_position(pos),
		m_color(color)
	{
	}
	
	void SetPosition(const glm::vec4& pos) { m_position = pos; }
	void SetColor(const glm::vec4& color) { m_color = color; }
	glm::vec4 GetPosition() const noexcept{ return m_position; }
	glm::vec4 GetColor() const noexcept { return m_color; }
protected:

	glm::vec4 m_position;
	glm::vec4 m_color;
};

struct PointLight 
{
public:
	glm::vec4 m_position;
	glm::vec4 m_color;
};
