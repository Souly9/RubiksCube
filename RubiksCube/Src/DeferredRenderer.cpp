#include "stdafx.h"
#include "DeferredRenderer.h"


#include "Program.h"
#include "Shapes.h"

DeferredRenderer::DeferredRenderer() :
m_screenQuad(ScreenQuad::CreateScreenQuad())
{
	glGenFramebuffers(1, &m_gBufferHandle);	
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferHandle);
	const auto x = WindowManager::Get().GetScreenWidth();
	const auto y = WindowManager::Get().GetScreenHeight();
	const auto pos = TextureManager::Get()->GetFrameBufferTexture(x, y, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0);
	const auto normal = TextureManager::Get()->GetFrameBufferTexture(x, y, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1);
	const auto albedoSpec = TextureManager::Get()->GetFrameBufferTexture(x, y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT2);
	m_textureHandles[0] = pos;
	m_textureHandles[1] = normal;
	m_textureHandles[2] = albedoSpec;
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glDrawBuffers(3, m_attachmentDesc);

	Shader deferredShader("DummyVert.vs", "DeferredLightingFrag.frag");

	std::vector<MaterialUtilities::UniformAttribute> uniforms;

	Properties props(glm::vec3(1,1,1),
	                 glm::vec3(1,1,1),
	                 glm::vec3(1,1,1),
	                 0.3f * 32);

	std::shared_ptr<TextureData> tmp = std::make_unique<TextureData>();
	std::vector<MaterialUtilities::ShaderAttribute> tmp2;
	const std::shared_ptr<Material> lightingMat = 
		std::make_shared<Material>(MaterialUtilities::MaterialPreset::Default, 
			deferredShader, Program::m_materialIndex++, props, tmp, tmp2);

	const auto screenQuad = std::make_shared<Mesh3D>(ScreenQuad::CreateScreenQuad());
	GeometryManager::Get()->AddGeometry(screenQuad, *lightingMat);

	m_lightingBatch = RenderBatch(lightingMat, DataHolder::Get()->GetCameraShared());
	m_lightingBatch.CollectGeometry();
	lightingMat->BindShader();
	lightingMat->GetShader().SetARB("positionBuffer", pos);
	lightingMat->GetShader().SetARB("normalBuffer", normal);
	lightingMat->GetShader().SetARB("albedoBuffer", albedoSpec);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::Render(RenderBatch* batch)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferHandle);
	
	Renderer::Render(batch);
}

void DeferredRenderer::Assemble()
{
	// Render the G-Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_lightingBatch.BindShader();
	m_lightingBatch.BindBatchUniforms();
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void DeferredRenderer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferHandle);
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}
