#pragma once

#include "./buffer.h"
#include "./vertex.h"
#include "./sprite.h"

#include <vector>

namespace ns {

    struct SpriteBufferIndices {
        std::vector<uint16_t> indices;
        unsigned index_count = 0;

        Buffer ibo;

        SpriteBufferIndices();
        void update(unsigned sprite_count);
    };

    struct SpriteBuffer {
        std::vector<Vertex> vertices;

        unsigned sprite_count = 0;
        Buffer vbo;
        int texture_size[2] {-1, -1};

        SpriteBuffer();

        void clear();
        void append(const Sprite &spr);

        void draw(SpriteBufferIndices &indices) const;
    };
}
