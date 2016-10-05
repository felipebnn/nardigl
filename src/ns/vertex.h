#pragma once

#include <cstdint>

#include <glad/glad.h>

namespace ns {

    struct Vertex {
        float x, y;
        float u, v;
        uint8_t r, g, b, a;

        static void setupVertexAttribs(GLint pos, GLint uv, GLint color);
    };

}
