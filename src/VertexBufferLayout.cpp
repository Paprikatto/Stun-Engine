#include "VertexBufferLayout.h"
template<>
    void VertexBufferLayout::Push<float>(unsigned int count)
    {
        m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template<>
    void VertexBufferLayout::Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
