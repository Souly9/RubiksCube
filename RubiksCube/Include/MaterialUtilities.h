#pragma once
#include <map>
#include <glm/vec3.hpp>

struct TextureData
{
public:
	TextureData(const TextureData& data) = default;
	TextureData() = default;
	~TextureData() = default;
	uint64_t m_albedo;
	uint64_t m_displacement;
	uint64_t m_normalMap;
	uint64_t m_cubeMap;
};

struct Properties
{
public:
	Properties() = default;

	Properties(const glm::vec3 ambient,
	           const glm::vec3 diffuse,
	           const glm::vec3 specular,
	           const float shininess) noexcept : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
	                                             m_shininess(shininess)
	{
	}

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
};

namespace MaterialUtilities
{
	struct PresetConverter;
	static std::unique_ptr<PresetConverter> m_pSingleton;


	enum class MaterialPreset
	{
		Default
	};

	enum class ShaderAttribute
	{
		NORMALS,
		TEXCOORDS,
		VERTEXCOLOR,
		TANGENTSPACE
	};

	enum class UniformAttribute
	{
		ALBEDO,
		NORMALMAP,
		DISPLACEMENTMAP,
		CUBEMAP
	};

	struct PresetConverter
	{
	public:
		PresetConverter() = default;

		static PresetConverter* Get()
		{
			if (!m_pSingleton)
			{
				m_pSingleton = std::make_unique<PresetConverter>();
			}
			return m_pSingleton.get();
		}

		std::vector<ShaderAttribute> ConvertToSA(MaterialPreset preset)
		{
			return m_convertToSAMap[preset];
		}

		std::vector<UniformAttribute> ConvertToUA(MaterialPreset preset)
		{
			return m_convertToUAMap[preset];
		}

	private:
		PresetConverter(const PresetConverter& conv) = delete;

		std::map<MaterialPreset, std::vector<ShaderAttribute>> m_convertToSAMap;
		std::map<MaterialPreset, std::vector<UniformAttribute>> m_convertToUAMap;
	};
}
