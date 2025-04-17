module;

#include <OpenGL.pch>

module Easy.Core.Engine;

import Easy.Core.Basic;

void Easy::Engine::run() {
    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    std::cout << "OpenGL Version: " << glVersion << '\n';
    std::cout << "OpenGL Renderer: " << glRenderer << '\n';
}