#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
    vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(std::size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(uintptr_t)offset);
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
