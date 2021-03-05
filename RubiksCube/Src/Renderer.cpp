#include "stdafx.h"
#include "Renderer.h"

void Renderer::Render(RenderBatch* batch)
{
	batch->BindVAO();
	

	batch->BindShader();
	batch->BindBatchUniforms();
	for (unsigned int i = 0; i < batch->GetNumOfDrawCalls(); ++i)
	{
		batch->BindUniformsForDrawCall(i);
		const auto off = i > 0 ? batch->GetOffsetForCall(i - 1) : 0;
		glDrawElements(GL_TRIANGLES, batch->GetOffsetForCall(i), GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * off)); // NOLINT Can't cast via static cast
	}
}
