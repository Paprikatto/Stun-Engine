#include "VertexBuffer.h"

#include <iostream>

#include "Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //Create buffer that stores positions
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    if (m_RendererID != 0)
    {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
    }
}

void VertexBuffer::Bind() const
{
    std::cout << "Binding VertexBuffer with ID: " << m_RendererID << std::endl;
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    std::cout << "Unbinding VertexBuffer with ID: " << m_RendererID << std::endl;
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
