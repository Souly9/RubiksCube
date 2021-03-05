#include "stdafx.h"
#include "ForwardRenderer.h"

void ForwardRenderer::Render(RenderBatch* batch)
{
	Renderer::Render(batch);	
}

void ForwardRenderer::RenderInstanced(RenderBatch* batch)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	batch->BindVAO();

	batch->BindShader();
	batch->BindBatchUniforms();
	for (unsigned int i = 0; i < batch->GetNumOfDrawCalls(); ++i)
	{
		batch->BindUniformsForDrawCall(i);
		uint32_t off = i > 0 ? batch->GetOffsetForCall(i - 1) : 0;
		glDrawElements(GL_TRIANGLES, batch->GetOffsetForCall(i), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * off)); // NOLINT Can't cast via static cast
	}
}
