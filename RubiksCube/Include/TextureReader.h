#pragma once

class TextureReader
{
public:
	uint32_t GenTexture(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType);
	uint32_t GenTexture(const char* path, bool bSRGBFormat);
	uint32_t GenHDRTexture(const char* path);

	uint32_t GenCubemap(const char* path, bool bSRGBFormat);
};
