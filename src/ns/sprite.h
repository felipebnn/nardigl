#pragma once

#include "./spritematrix.h"
#include "./color.h"
#include "./rect.h"

namespace ns {

    struct Sprite {
        SpriteMatrix mat;
        IntRect rect;
        Color color = white;
    };

}
