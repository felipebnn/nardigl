#include "textureinfo.h"

#include "stb_image.h"
#include <memory>

#ifndef TEXTURE_MAX_ANISOTROPY_EXT
#define TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

namespace ns {

    TextureInfo loadTexture(int width, int height, const uint8_t *data) {
        TextureInfo ti;
        ti.width = width;
        ti.height = height;

        glGenTextures(1, &ti.handle.id);

        glBindTexture(GL_TEXTURE_2D, ti.handle.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); //TODO: ???

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        return ti;
    }

    TextureInfo loadTexture(const std::string &fileName) {
        int width, height, comp;

        auto data = std::unique_ptr<unsigned char[], void(*)(void*)> (
            stbi_load(fileName.c_str(), &width, &height, &comp, 4),
            &stbi_image_free
        );

        if (data == nullptr)
            return TextureInfo();

        return loadTexture(width, height, data.get());
    }

}
