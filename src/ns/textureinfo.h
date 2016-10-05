#pragma once

#include "./texture.h"

#include <string>

namespace ns {

    struct TextureInfo {
        Texture handle;
        int width, height;

        TextureInfo()
            : width(0), height(0)
        {}

        TextureInfo(TextureInfo &&o)
            : handle(std::move(o.handle))
        {}

        TextureInfo& operator= (TextureInfo &&o) {
            handle = std::move(o.handle);
            width = o.width;
            height = o.height;
            return *this;
        }

    private:
        TextureInfo(const TextureInfo&);
        TextureInfo& operator= (const TextureInfo&);
    };

    TextureInfo loadTexture(int width, int height, const uint8_t *data);
    TextureInfo loadTexture(const std::string &fileName);

}
