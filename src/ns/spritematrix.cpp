#include "spritematrix.h"

namespace ns {

    SpriteMatrix::SpriteMatrix() {
        identity();
    }

    SpriteMatrix& SpriteMatrix::identity() {
        m[0][0] = 1.0f;   m[0][1] = 0.0f;   m[0][2] = 0.0f;
        m[1][0] = 0.0f;   m[1][1] = 1.0f;   m[1][2] = 0.0f;

        return *this;
    }

    SpriteMatrix& SpriteMatrix::multiply(const SpriteMatrix &o) {
        float a = m[0][0],   b = m[0][1],   c = m[0][2];
        float d = m[1][0],   e = m[1][1],   f = m[1][2];

        float A = o.m[0][0],   B = o.m[0][1],   C = o.m[0][2];
        float D = o.m[1][0],   E = o.m[1][1],   F = o.m[1][2];

        m[0][0] = a*A + b*D;   m[0][1] = a*B + b*E;   m[0][2] = a*C + b*F + c;
        m[1][0] = d*A + e*D;   m[1][1] = d*B + e*E;   m[1][2] = d*C + e*F + f;

        return *this;
    }

    SpriteMatrix& SpriteMatrix::rotate(const Complex &z) {
        float a = m[0][0],   b = m[0][1],   c = m[0][2];
        float d = m[1][0],   e = m[1][1],   f = m[1][2];

        m[0][0] = a*z.r - d*z.i;   m[0][1] = b*z.r - e*z.i;   m[0][2] = c*z.r - f*z.i;
        m[1][0] = a*z.i + d*z.r;   m[1][1] = b*z.i + e*z.r;   m[1][2] = c*z.i + f*z.r;

        return *this;
    }

    SpriteMatrix& SpriteMatrix::scale(const vec2 &v) {
        m[0][0] *= v[0];   m[0][1] *= v[0];   m[0][2] *= v[0];
        m[1][0] *= v[1];   m[1][1] *= v[1];   m[1][2] *= v[1];

        return *this;
    }

    SpriteMatrix& SpriteMatrix::shear(const vec2 &v) {
        float a = m[0][0],   b = m[0][1],   c = m[0][2];
        float d = m[1][0],   e = m[1][1],   f = m[1][2];

        m[0][0] += v[0] * d;   m[0][1] += v[0] * e;   m[0][2] += v[0] * f;
        m[1][0] += v[1] * a;   m[1][1] += v[1] * b;   m[1][2] += v[1] * c;

        return *this;
    }

    SpriteMatrix& SpriteMatrix::translate(const vec2 &v) {
        m[0][2] += v[0];
        m[1][2] += v[1];

        return *this;
    }

    vec2 SpriteMatrix::transform(const vec2 &v) const {
        return {{
            m[0][0]*v[0] + m[0][1]*v[1] + m[0][2],
            m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]
        }};
    }

}
