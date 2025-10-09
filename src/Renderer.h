#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_CALL(x) GLClearError(); x; ASSERT(GLCheckError(#x, __FILE__, __LINE__));

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
class Renderer
{
private:
   glm::vec3 m_BackgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
public:
   void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
   void Clear() const;
   void SetBackgroundColor(const glm::vec3& color) { m_BackgroundColor = color; }
   [[nodiscard]] glm::vec3 GetBackgroundColor() const { return m_BackgroundColor; }
};
