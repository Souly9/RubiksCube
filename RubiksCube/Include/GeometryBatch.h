#pragma once
#include "MaterialUtilities.h"

struct TextureData;
class VertexAttribPointer;

class GeometryBatch
{
public:

	/*
	 * Binds this buffer as VAO and EBO
	 */
	void Bind();
	void Clear();


	void GenBuffers();

	void FillBuffer(const std::vector<std::shared_ptr<Mesh3D>>& geometry,
	                const std::vector<MaterialUtilities::ShaderAttribute>& attributes,
	                std::shared_ptr<TextureData> texData);
	uint64_t GetNumOfPrimitives() const;
	uint32_t GetOffsetForCall(uint32_t drawCallIndex);

	void BindBuffers();

	TextureData* GetTexData() const;
private:
	void AddTwoFloats(const std::vector<glm::vec2>& floats);
	void AddThreeFloats(const std::vector<glm::vec3>& floats);

	/*
	 * Number of primitives in the buffer per object
	 * Each entry signals one object, each value the offset into the buffer
	 * Only one offset needed since we operate under the assumption that every Vertex has every attribute
	 */
	std::vector<uint32_t> m_numOfPrimitives;
	std::vector<uint32_t> m_indices;
	std::vector<VertexAttribPointer> m_attribPointers;
	std::vector<float> m_buffer;

	std::shared_ptr<TextureData> m_texData;

	uint64_t m_bufferElements{};
	uint32_t m_EBOHandle{};
	uint32_t m_BufferHandle{};
	uint32_t m_textureBufferHandle{};

	bool m_isBound{false};
	bool m_buffersGenerated{false};
};
