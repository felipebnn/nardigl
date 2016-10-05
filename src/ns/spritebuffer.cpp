#include "spritebuffer.h"

#include <cassert>

namespace ns {

    SpriteBufferIndices::SpriteBufferIndices() {
        glGenBuffers(1, &ibo.id);
    }

    void SpriteBufferIndices::update(unsigned sprite_count) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);

        if (index_count >= sprite_count)
            return;

        indices.reserve(sprite_count * 6);

        for (unsigned i = index_count; i < sprite_count; ++i) {
            uint16_t base = 4*i;

            indices.push_back(base + 0);
            indices.push_back(base + 1);
            indices.push_back(base + 2);

            indices.push_back(base + 0);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
        }

        index_count = sprite_count;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STREAM_DRAW);
    }

    SpriteBuffer::SpriteBuffer() {
        glGenBuffers(1, &vbo.id);
    }

    void SpriteBuffer::clear() {
        vertices.clear();
        sprite_count = 0;
    }

    void SpriteBuffer::append(const Sprite &spr) {
        assert(texture_size[0] >= 0 && texture_size[1] >= 0);

        float spr_w = spr.rect.w;
        float spr_h = spr.rect.h;

        float img_x = spr.rect.x / (float) texture_size[0];
        float img_w = spr.rect.w / (float) texture_size[0];
        float img_y = spr.rect.y / (float) texture_size[1];
        float img_h = spr.rect.h / (float) texture_size[1];

        Vertex v;
        v.r = spr.color.r; //TODO: do sum thing?
        v.g = spr.color.g;
        v.b = spr.color.b;
        v.a = spr.color.a;

        auto p = spr.mat.transform({{0.0f, 0.0f}});
        v.x = p[0];
        v.y = p[1];
        v.u = img_x;
        v.v = img_y;
        vertices.push_back(v);

        p = spr.mat.transform({{spr_w, 0.0f}});
        v.x = p[0];
        v.y = p[1];
        v.u = img_x + img_w;
        //v.v = img_y;
        vertices.push_back(v);

        p = spr.mat.transform({{spr_w, spr_h}});
        v.x = p[0];
        v.y = p[1];
        //v.u = img_x + img_w;
        v.v = img_y + img_h;
        vertices.push_back(v);

        p = spr.mat.transform({{0.0f, spr_h}});
        v.x = p[0];
        v.y = p[1];
        v.u = img_x;
        //v.v = img_y + img_h;
        vertices.push_back(v);

        ++sprite_count;
    }

    void SpriteBuffer::draw(SpriteBufferIndices &indices) const {
        indices.update(sprite_count);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STREAM_DRAW);
        //Vertex::setupVertexAttribs(0); //TODO: comofas?
        glDrawElements(GL_TRIANGLES, sprite_count * 6, GL_UNSIGNED_SHORT, nullptr);
    }

}
