#pragma once

#include <glad/glad.h>

#include <utility>

namespace ns {

    struct Texture {
        GLuint id;

        Texture()
            : id(0)
        {}

        explicit Texture(GLuint id)
            : id(id)
        {}

        Texture(Texture &&o)
            : id(o.id)
        {
            o.id = 0;
        }

        Texture& operator= (Texture &&o) {
            std::swap(id, o.id);
            return *this;
        }

        ~Texture() {
            if (id)
                glDeleteBuffers(1, &id);
        }

    private:
        Texture(const Texture&);
        Texture& operator= (const Texture&);
    };

}
