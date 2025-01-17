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