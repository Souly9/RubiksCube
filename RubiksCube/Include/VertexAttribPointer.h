#pragma once
#include "stdafx.h"

class VertexAttribPointer
{
public:
	VertexAttribPointer(uint64_t chunkLength, uint64_t attribIndex, uint64_t offset) noexcept : m_attribIndex(attribIndex), m_chunkLength(chunkLength), m_offset(offset) 
	{
	}

	void BindPointer()
	{
		glVertexAttribPointer(m_attribIndex, m_chunkLength, GL_FLOAT, GL_FALSE, m_chunkLength * sizeof(float), (void*)(m_offset * sizeof(float)));
		glEnableVertexAttribArray(m_attribIndex);
	}
	
protected:
	uint64_t m_attribIndex;
	uint64_t m_chunkLength;
	uint64_t m_offset;
};

