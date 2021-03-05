#pragma once
#include <InternalStates.h>

class Material;
class Camera;

/*
 * Class that groups together all calls for one material
 */
class RenderBatch
{
public:
	RenderBatch() = default;
	RenderBatch(std::shared_ptr<Material> mat, std::shared_ptr<Camera> cam);

	/*
	* Collects the geometry for its material and fills the buffer 
	* Expensive operation as it fills the buffer by copying the geometry
	*/
	void CollectGeometry();
	void BindBatchUniforms();
	void BindShader();
	void BindVAO();

	/**
	 * Returns the number of primitives this batch contains and needs to be drawn
	 */
	uint64_t NumToDraw() const { return m_geometryBuffer->GetNumOfPrimitives(); }

	void SetCamera(std::shared_ptr<Camera>& cam);
	uint32_t GetNumOfDrawCalls() const noexcept { return m_drawCallsToIssue; }
	void BindUniformsForDrawCall(uint32_t drawCall);
	uint32_t GetOffsetForCall(uint32_t drawCall) const { return m_geometryBuffer->GetOffsetForCall(drawCall); }

private:
	RenderBatchState m_state{TextureUpdate};
	std::vector<glm::mat4> m_modelBuffer;

	unsigned int m_VAO;

	std::unique_ptr<GeometryBatch> m_geometryBuffer;
	std::vector<std::shared_ptr<Mesh3D>> m_geometry;
	std::shared_ptr<Material> m_myMaterial;

	std::shared_ptr<Camera> m_pMainCamera;

	uint32_t m_drawCallsToIssue{};
};
