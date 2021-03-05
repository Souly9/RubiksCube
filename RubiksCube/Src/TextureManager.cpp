#include "stdafx.h"
#include <TextureReader.h>

static std::unique_ptr<TextureManager> m_pSingleton;

TextureManager::TextureManager() :
	m_textureHandles{},
	m_freePos(&m_textureHandles[0]),
	m_reader(std::make_unique<TextureReader>())
{
}

TextureManager::~TextureManager()
{
	for(auto& handle : m_textureHandles)
	{
		ReleaseTexture(handle);
	}
}

TextureManager* TextureManager::Get()
{
	if (!m_pSingleton)
	{
		m_pSingleton = std::make_unique<TextureManager>();
	}
	return m_pSingleton.get();
}

uint64_t TextureManager::GetTextureHandle(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType)
{
	auto tex = m_reader->GenTexture(x, y, type, channelTypes, valueType);
	auto handle = glGetTextureHandleARB(tex);
	MakeTextureResident(handle);
	
	return handle;
}

uint64_t TextureManager::GetFrameBufferTexture(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType,
	GLenum target)
{
	auto tex = m_reader->GenTexture(x, y, type, channelTypes, valueType);
	glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, tex, 0);
	auto handle = glGetTextureHandleARB(tex);
	MakeTextureResident(handle);
	
	return handle;
}

uint64_t TextureManager::GetTextureHandle(const char* path)
{
	uint32_t tex = m_reader->GenTexture(path);
	uint64_t handle = glGetTextureHandleARB(tex);
	MakeTextureResident(handle);
	
	return handle;
}


uint64_t TextureManager::GetCubemapHandle(const char* path)
{
	uint32_t tex = m_reader->GenCubemap(path);
	uint64_t handle = glGetTextureHandleARB(tex);

	MakeTextureResident(handle);
	return handle;
}

void TextureManager::MakeTextureResident(uint64_t textureHandle)
{
	if (m_freePos != &m_textureHandles[COMBUSTION_TEXTURE_LIMIT])
	{
		*m_freePos = textureHandle;
		m_freePos++;
		glMakeTextureHandleResidentARB(textureHandle);
	}
	else
	{
		std::cout << "Texture Limit reached" << '\n';
	}
}

void TextureManager::ReleaseTexture(uint64_t arbTextureHandle)
{
	glMakeTextureHandleNonResidentARB(arbTextureHandle);
}
