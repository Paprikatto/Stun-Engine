#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_CALL(x) GLClearError(); x; ASSERT(GLCheckError(#x, __FILE__, __LINE__));

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
class Renderer
{
public:
   void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
   void Clear() const;
};
