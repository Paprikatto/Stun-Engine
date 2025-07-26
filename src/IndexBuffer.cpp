#include "IndexBuffer.h"

#include <iostream>

#include "Renderer.h"
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): m_Count(count)
{
    if (!data) std::cout << "data is null!" << std::endl;
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    //Create buffer that stores positions
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    if (m_RendererID != 0)
    {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
    }
}

void IndexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
