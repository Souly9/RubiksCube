#pragma once

class TextureReader;
typedef unsigned int GLenum;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	static TextureManager* Get();
	uint64_t GetTextureHandle(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType);
	uint64_t GetTextureHandle(const char* path);
	uint64_t GetCubemapHandle(const char* path);
	uint64_t GetFrameBufferTexture(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType, GLenum target);
	void MakeTextureResident(uint64_t textureHandle);
	static void ReleaseTexture(uint64_t arbTextureHandle);
	void RemoveTexture(uint64_t handle);
private:
	uint64_t m_textureHandles[COMBUSTION_TEXTURE_LIMIT];
	unsigned int m_freeSlots;
	std::unique_ptr<TextureReader> m_reader;
};


