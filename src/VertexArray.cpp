#include "VertexArray.h"

#include <iostream>
#include <bits/shared_ptr.h>

#include "Renderer.h"

VertexArray::VertexArray():m_RendererID(0)
{
	GL_CALL(glGenVertexArrays(1, &m_RendererID));
	std::cout << "Creating VertexArray with id "<< m_RendererID << std::endl;
}

VertexArray::~VertexArray()
{
	if (m_RendererID != 0)
	{
		GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
	}
}

void VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer>& vb, const VertexBufferLayout& layout) const
{
	Bind();
    vb->Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(std::size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(uintptr_t)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GL_CALL(glBindVertexArray(0));
}
