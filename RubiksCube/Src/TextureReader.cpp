#include "stdafx.h"
#include "TextureReader.h"

#include "stb_image.h"

uint32_t TextureReader::GenTexture(int x, int y, GLenum type, GLenum channelTypes, GLenum valueType)
{
	uint32_t handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, type, x, y, 0, channelTypes, valueType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return handle;
}

uint32_t TextureReader::GenTexture(const char* path, bool bSRGBFormat)
{
	int32_t width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	if (!data)
	{
		std::cout << "Image at " << path << " not loaded correctly" << '\n';
		return 0;
	}

	uint32_t tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (nrChannels == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bSRGBFormat ? GL_SRGB : GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bSRGBFormat ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	if (nrChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bSRGBFormat ? GL_SRGB_ALPHA : GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return tex;
}

uint32_t TextureReader::GenHDRTexture(const char* path)
{
	int32_t width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path, &width, &height, &nrChannels, 0);

	if (!data)
	{
		std::cout << "Image at " << path << " not loaded correctly" << '\n';
	}

	uint32_t tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return tex;
}

uint32_t TextureReader::GenCubemap(const char* path, bool bSRGBFormat)
{
	std::vector<const char*> faces
	{
		"posx.jpg",
		"negx.jpg",
		"posy.jpg",
		"negy.jpg",
		"posz.jpg",
		"negz.jpg"
	};
	int32_t width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	
	uint32_t tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(
			std::string(path).append(faces.at(i)).c_str(), 
			&width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, bSRGBFormat ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		if (!data)
		{
			std::cout << "Image at " << path << " not loaded correctly" << '\n';
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return tex;
}
