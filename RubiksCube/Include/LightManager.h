#pragma once
#include "LightStructs.h"

class ShaderResource;

class LightManager
{
public:
	LightManager(float ambientStr);

	void AddLight(PointLight& light);
	void AddLights(std::vector<PointLight> lights);

	void RemoveLight(PointLight light);
	void RemoveAtIndex(int index);

	bool InitializeLightBuffer();
	bool SubmitLightBuffer();

	void MultiplyLights(glm::mat4 mat);
private:
	struct LightData
	{
		LightData(float ambient) : m_ambientStrength(ambient) {}
		
		glm::vec4 m_ambientStrength;
		std::vector<PointLight> m_pointLights;
	};
	std::vector<PointLight> m_pointLights;
	std::unique_ptr<LightData> m_pBuffer;
	// Index used to refer to the constant buffer assigned to this manager
	uint32_t m_bufferIndex = 1;
	uint32_t m_bufferHandle;
	bool m_isDirty;
	bool m_isInitialized;
};

