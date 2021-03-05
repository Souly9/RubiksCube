#include "stdafx.h"
#include <RenderBatch.h>

#include <utility>

bool m_tmp = false;

RenderBatch::RenderBatch(std::shared_ptr<Material> mat, std::shared_ptr<Camera> cam)
	: m_geometryBuffer(std::make_unique<GeometryBatch>())
	  , m_myMaterial(std::move(mat))
	  , m_pMainCamera(std::move(cam))
{
	glGenVertexArrays(1, &m_VAO);
}

void RenderBatch::CollectGeometry()
{
	glBindVertexArray(m_VAO);
	m_geometry.clear();
	m_geometry = GeometryManager::Get()->GetGeometryForMaterial(*m_myMaterial);
	m_geometryBuffer->FillBuffer(m_geometry, m_myMaterial->GetShaderAttributes(), m_myMaterial->GetHandles());
	m_drawCallsToIssue = m_geometry.size();
}

void RenderBatch::BindBatchUniforms()
{
	m_myMaterial->BindProperties();
	m_geometryBuffer->BindBuffers();


	if (m_state == TextureUpdate)
	{
		const TextureData* d = m_geometryBuffer->GetTexData();
		if(d->m_albedo != 0)
			m_myMaterial->GetShader().SetARB("opaque", d->m_albedo);
		if(d->m_displacement != 0)
			m_myMaterial->GetShader().SetARB("displacement", d->m_displacement);
		if(d->m_normalMap != 0)
			m_myMaterial->GetShader().SetARB("normalMap", d->m_normalMap);
		if(d->m_cubeMap != 0)
			m_myMaterial->GetShader().SetARB("skyBox", d->m_normalMap);
		m_state = NoUpdate;
	}
}


void RenderBatch::BindShader()
{
	m_myMaterial->BindShader();
}

void RenderBatch::BindVAO()
{
	glBindVertexArray(m_VAO);
}

void RenderBatch::SetCamera(std::shared_ptr<Camera>& cam)
{
	m_pMainCamera = std::move(cam);
}

void RenderBatch::BindUniformsForDrawCall(uint32_t drawCall)
{
	m_myMaterial->GetShader().SetMat4("modelMat", m_geometry.at(drawCall)->GetModel());
}
