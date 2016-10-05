#pragma once

#include <glad/glad.h>

#include <utility>

namespace ns {

    struct Buffer {
        GLuint id;

        Buffer()
            : id(0)
        {}

        explicit Buffer(GLuint id)
            : id(id)
        {}

        Buffer(Buffer &&o)
            : id(o.id)
        {
            o.id = 0;
        }

        Buffer& operator= (Buffer &&o) {
            std::swap(id, o.id);
            return *this;
        }

        ~Buffer() {
            if (id)
                glDeleteBuffers(1, &id);
        }

    private:
        Buffer(const Buffer&);
        Buffer& operator= (const Buffer&);
    };

}
