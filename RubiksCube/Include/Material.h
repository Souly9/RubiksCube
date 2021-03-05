#pragma once
#include "Shader.h"

class Shader;
struct TextureData;

class Material
{
public:
	Material() = default;
	Material(MaterialUtilities::MaterialPreset def, Shader& shader, uint32_t id);

	Material(MaterialUtilities::MaterialPreset def,
	         const Shader& shader,
	         uint32_t id,
	         const Properties& props,
	         std::shared_ptr<TextureData> handles,
	         std::vector<MaterialUtilities::ShaderAttribute> attbs);

	~Material()
	{
	}


	bool operator==(const Material& mat) const noexcept
	{
		return m_myShaderProgram.ID == mat.m_myShaderProgram.ID;
	}

	auto GetShaderAttributes() const -> std::vector<MaterialUtilities::ShaderAttribute> { return m_shaderAttributes; }

	uint32_t GetID() const noexcept
	{
		return m_ID;
	}

	void BindShader()
	{
		m_myShaderProgram.Use();
	}

	Shader GetShader() const noexcept
	{
		return m_myShaderProgram;
	}

	std::shared_ptr<TextureData> GetHandles() const noexcept;
	void BindProperties();
private:
	uint32_t m_ID;
	Shader m_myShaderProgram;


	Properties m_properties;
	std::vector<MaterialUtilities::ShaderAttribute> m_shaderAttributes;
	std::vector<MaterialUtilities::UniformAttribute> m_uniformAttributes;


	std::shared_ptr<TextureData> m_textures;
};
