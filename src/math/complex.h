#pragma once

#include "./vec.h"

namespace ns {

    struct Complex {
        float r, i;

        Complex()
            : r(1.0f), i(0.0f)
        {}

        Complex(float r, float i)
            : r(r), i(i)
        {}

        Complex(const vec2 &v)
            : r(v[0]), i(v[1])
        {}

        inline static Complex from_angle(float radians) {
            return Complex(std::cos(radians), std::sin(radians));
        }

        inline vec2 as_vec2() const {
            return vec2{{r, i}};
        }
    };

    inline Complex operator* (const Complex &a, const Complex &b) {
        return Complex(a.r*b.r - a.i*b.i, a.i*b.r + a.r*b.i);
    }

    inline Complex conjugate(const Complex &a) {
        return Complex(a.r, -a.i);
    }

    inline bool linearly_dependent(const vec2 &a, const vec2 &b) {
        return a[0]*b[1] - a[1]*b[0] < 1e-4f;
    }

    vec2 slerp(const vec2 &a, vec2 b, float t);
    vec2 rotateTowards(const vec2 &a, vec2 b, float max_angle);

}
