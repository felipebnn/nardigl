#include "vertex.h"

namespace ns {

    void Vertex::setupVertexAttribs(GLint pos, GLint uv, GLint color) {
        glVertexAttribPointer(pos, 2, GL_FLOAT, false, sizeof(ns::Vertex), (void*)offsetof(ns::Vertex, x));
        glEnableVertexAttribArray(pos);

        glVertexAttribPointer(uv, 2, GL_FLOAT, false, sizeof(ns::Vertex), (void*)offsetof(ns::Vertex, u));
        glEnableVertexAttribArray(uv);

        glVertexAttribPointer(color, 4, GL_UNSIGNED_BYTE, true, sizeof(ns::Vertex), (void*)offsetof(ns::Vertex, r));
        glEnableVertexAttribArray(color);
    }

}
