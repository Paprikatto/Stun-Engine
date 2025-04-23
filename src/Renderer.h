#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <csignal>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #define DEBUG_BREAK() ((void)0)  // fallback: do nothing
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK()

#define GL_CALL(x) GLClearError(); x; ASSERT(GLCheckError(#x, __FILE__, __LINE__))
void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
class Renderer
{
public:
   void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
   void Clear() const;
};
