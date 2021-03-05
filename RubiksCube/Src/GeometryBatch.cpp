#include "stdafx.h"
#include "GeometryBatch.h"

using namespace MaterialUtilities;

void GeometryBatch::Bind()
{
}

void GeometryBatch::Clear()
{
	m_buffer.clear();
	m_indices.clear();
}

void GeometryBatch::FillBuffer(const std::vector<std::shared_ptr<Mesh3D>>& geometry,
                               const std::vector<ShaderAttribute>& attributes,
                               std::shared_ptr<TextureData> texData)
{
	if (!m_buffersGenerated)
	{
		GenBuffers();
	}

	Clear();
	m_texData = std::move(texData);
	uint64_t bufferElements = 0;
	for (auto mesh : geometry)
	{
		bufferElements += mesh->GetData()->m_positions.size();
		m_numOfPrimitives.push_back(mesh->GetPrimimitveNum());
		m_bufferElements += mesh->GetPrimimitveNum();
	}
	m_buffer.reserve(m_bufferElements * 3);
	for (uint32_t i = 0; i < m_numOfPrimitives.size(); ++i)
	{
		AddThreeFloats(geometry.at(i)->GetData()->m_positions);
	}
	for (uint32_t i = 0; i < m_numOfPrimitives.size(); ++i)
	{
		const uint32_t off = i > 0 ? geometry.at(i - 1)->GetData()->m_positions.size() : 0;

		for (uint32_t a = 0; a < m_numOfPrimitives.at(i); ++a)
		{
			m_indices.push_back(geometry.at(i)->GetData()->m_indices.at(a) + off);
		}
	}


	// Initialize the pointer for the vertex positions
	uint64_t chunkLength = 3, index = 0;
	m_attribPointers.emplace_back(chunkLength, index, 0);
	index++;
	uint64_t offset = bufferElements * chunkLength * index;
	{
		// Initialize any optional pointers
		for (uint32_t a = 0; a < attributes.size(); ++a)
		{
			switch (attributes.at(a))
			{
			case(ShaderAttribute::NORMALS):
				{
					chunkLength = 3;
					for (uint32_t i = 0; i < geometry.size(); ++i)
					{
						AddThreeFloats(geometry.at(i)->GetData()->m_normals);
					}
					break;
				}
			case(ShaderAttribute::VERTEXCOLOR):
				{
					chunkLength = 3;
					for (uint32_t i = 0; i < geometry.size(); ++i)
					{
						AddThreeFloats(geometry.at(i)->GetData()->m_colors);
					}
					break;
				}
			case(ShaderAttribute::TANGENTSPACE):
				{
					chunkLength = 3;
					for (uint32_t i = 0; i < geometry.size(); ++i)
					{
						AddThreeFloats(geometry.at(i)->GetData()->m_tangents);
					}

					VertexAttribPointer tmp(chunkLength, index, offset);
					offset += bufferElements * chunkLength;
					tmp.BindPointer();
					m_attribPointers.push_back(tmp);
					index++;

					for (uint32_t i = 0; i < geometry.size(); ++i)
					{
						AddThreeFloats(geometry.at(i)->GetData()->m_biTangents);
					}
					break;
				}
			case(ShaderAttribute::TEXCOORDS):
				{
					chunkLength = 2;
					for (uint32_t i = 0; i < geometry.size(); ++i)
					{
						AddTwoFloats(geometry.at(i)->GetData()->m_texCoords);
					}
					break;
				}
			}

			const VertexAttribPointer tmp(chunkLength, index, offset);
			offset += bufferElements * chunkLength;
			m_attribPointers.push_back(tmp);
			index++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_buffer.size(), m_buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	for (auto& pointer : m_attribPointers)
	{
		pointer.BindPointer();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint64_t GeometryBatch::GetNumOfPrimitives() const
{
	return m_indices.size();
}

void GeometryBatch::GenBuffers()
{
	glGenBuffers(1, &m_BufferHandle);
	glGenBuffers(1, &m_EBOHandle);
	glGenBuffers(1, &m_textureBufferHandle);
	m_buffersGenerated = true;
}

uint32_t GeometryBatch::GetOffsetForCall(const uint32_t drawCallIndex)
{
	uint32_t offset = 0;
	for (uint32_t i = 0; i < drawCallIndex; ++i)
	{
		offset += m_numOfPrimitives.at(i);
	}
	return m_numOfPrimitives.at(drawCallIndex);
}

void GeometryBatch::BindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOHandle);
}

TextureData* GeometryBatch::GetTexData() const
{
	return m_texData.get();
}

void GeometryBatch::AddTwoFloats(const std::vector<glm::vec2>& floats)
{
	for (const auto& vec : floats)
	{
		m_buffer.push_back(vec.x);
		m_buffer.push_back(vec.y);
	}
}

void GeometryBatch::AddThreeFloats(const std::vector<glm::vec3>& floats)
{
	for (const auto& vec : floats)
	{
		m_buffer.push_back(vec.x);
		m_buffer.push_back(vec.y);
		m_buffer.push_back(vec.z);
	}
}
