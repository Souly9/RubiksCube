#pragma once
#include <unordered_map>

class TextureReader;
typedef unsigned int GLenum;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	static TextureManager* Get();
	uint64_t GetTextureHandle(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType);
	uint64_t GetTextureHandle(const char* path, bool bSRGBFormat);
	uint64_t GetCubemapHandle(const char* path, bool bSRGBFormat);
	uint64_t GetFrameBufferTexture(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType, GLenum target);
	void MakeTextureResident(uint64_t textureHandle);
	static void ReleaseTexture(uint64_t arbTextureHandle);

private:
	std::unique_ptr<TextureReader> m_reader;
	std::unordered_map<std::string, uint64_t> m_paths;
};


