#pragma once
#include "Renderer.h"

class DeferredRenderer : public Renderer
{
	unsigned int m_gBufferHandle;
	unsigned int m_attachmentDesc[3]
	{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	uint32_t texHandles[3];
	RenderBatch m_lightingBatch;
	Mesh3D m_screenQuad;
public:
	DeferredRenderer();
	void Render(RenderBatch* batch) override;
	void Assemble();
	void Clear();
};

