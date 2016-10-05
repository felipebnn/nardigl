#include "text.h"

#include <cassert>

namespace ns {

    int measureStringWidth(const std::string &txt, const FontInfo &font) {
        return txt.length() * font.char_w;
    }

    void drawString(int x, int y, const std::string &txt, SpriteBuffer &buffer, const FontInfo &font, const Color &color) {
        Sprite spr;
        spr.mat.translate(vec2{(float)x, (float)y});
        spr.rect = IntRect{ 0, 0, font.char_w, font.char_h };
        spr.color = color;

        for (char c : txt) {
            const int grid_pos  = c - font.first_char;
            const int grid_line = grid_pos / font.grid_w;
            const int grid_col  = grid_pos % font.grid_w;
            assert(grid_line < font.grid_h);

            spr.rect.x = font.img_x + grid_col * font.char_w;
            spr.rect.y = font.img_y + grid_line * font.char_h;
            buffer.append(spr);
            spr.mat.translate(vec2{(float)font.char_w, 0.0f});
        }
    }

    void drawString(int x, int y, const std::string &txt, SpriteBuffer &buffer, const FontInfo &font, const TextAlignment &alignment, const Color &color) {
        switch (alignment) {
        case TextAlignment::left:
            break;
        case TextAlignment::right:
            x = x - measureStringWidth(txt, font);
            break;
        case TextAlignment::center:
            /*x = x - measureStringWidth(txt, font)/2; porem arredondado para baixo*/
            x = (2*x - measureStringWidth(txt, font)) / 2;
            break;
        }

        drawString(x, y, txt, buffer, font, color);
    }

}
