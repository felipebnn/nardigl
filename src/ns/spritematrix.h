#pragma once

#include "math/complex.h"
#include "math/vec.h"

namespace ns {

    struct SpriteMatrix {
        float m[2][3];

        SpriteMatrix();

        SpriteMatrix& identity();
        SpriteMatrix& multiply(const SpriteMatrix &o);
        SpriteMatrix& rotate(const Complex &z);
        SpriteMatrix& scale(const vec2 &v);
        SpriteMatrix& shear(const vec2 &v);
        SpriteMatrix& translate(const vec2 &v);

        vec2 transform(const vec2 &v) const;
    };

}
