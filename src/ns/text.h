#pragma once

#include <string>
#include "ns/spritebuffer.h"

namespace ns {

    struct FontInfo {
        char first_char;
        int char_w, char_h;
        int img_x, img_y;
        int grid_w, grid_h; //in characters
    };

    enum class TextAlignment {
        left, right, center
    };

    int measureStringWidth(const std::string &txt, const FontInfo &font);
    void drawString(int x, int y, const std::string &txt, SpriteBuffer &buffer, const FontInfo &font, const Color &color);
    void drawString(int x, int y, const std::string &txt, SpriteBuffer &buffer, const FontInfo &font, const TextAlignment &alignment, const Color &color);

}
