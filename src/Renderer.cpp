#include "Renderer.h"
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}
bool GLCheckError(const char* function, const char* file, int line) {
    bool success = true;
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << '\n';
        success = false;
	}
    return success;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    	shader.Bind();
    	va.Bind();
    	ib.Bind();
    	GL_CALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
