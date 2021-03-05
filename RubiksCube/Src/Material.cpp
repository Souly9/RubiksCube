#include "stdafx.h"
#include "Material.h"

Material::Material(MaterialUtilities::MaterialPreset def, Shader& shader, uint32_t id) :
	m_ID{id},
	m_myShaderProgram{shader},
	m_properties{},
	m_shaderAttributes{MaterialUtilities::PresetConverter::Get()->ConvertToSA(def)},
	m_uniformAttributes{MaterialUtilities::PresetConverter::Get()->ConvertToUA(def)}
{
}

Material::Material(const MaterialUtilities::MaterialPreset def, const Shader& shader, const uint32_t id,
                   const Properties& props, const std::shared_ptr<TextureData> handles,
                   const std::vector<MaterialUtilities::ShaderAttribute> attbs) :
	m_ID(id),
	m_myShaderProgram(shader),
	m_properties(props),
	m_shaderAttributes(attbs), m_uniformAttributes(MaterialUtilities::PresetConverter::Get()->ConvertToUA(def)),
	m_textures(handles)
{
}

std::shared_ptr<TextureData> Material::GetHandles() const noexcept
{
	return m_textures;
}

void Material::BindProperties()
{
	m_myShaderProgram.SetVec3("material.ambient", m_properties.m_ambient);
	m_myShaderProgram.SetVec3("material.diffuse", m_properties.m_diffuse);
	m_myShaderProgram.SetVec3("material.specular", m_properties.m_specular);
	m_myShaderProgram.SetFloat("material.shininess", m_properties.m_shininess);
}
