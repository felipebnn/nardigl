#include "complex.h"

#include <algorithm>

#ifndef clamp
#define clamp(a,x,b) std::max(a,std::min(x,b))
#endif

namespace ns {

    vec2 slerp(const vec2 &a, vec2 b, float t) {
        //return other if eigher is NaN
        if (!(b == b)) return a; //TODO: test
        if (!(a == a)) return b;

        if (linearly_dependent(a, b)) {
            b[0] += 1e-3f * a[0]; //jiggle jiggle
            b[1] -= 1e-3f * a[1];
            b = normalized(b);
        }

        float cos_theta = dot(a,b);
        float theta = std::acos(clamp(0.0f, cos_theta, 1.0f));

        //both point the same way
        if (theta < 1e-3f)
            return a;

        if (cos_theta > 0.9995f) {
            return nlerp(a, b, t);
        } else {
            float theta_t = theta * t;

            vec2 c = normalized(b - a*cos_theta);
            return a*std::cos(theta_t) + c*std::sin(theta_t);
        }
    }

    vec2 rotateTowards(const vec2 &a, vec2 b, float max_angle) {
        //return other if eigher is NaN
        if (!(b == b)) return a; //TODO: test
        if (!(a == a)) return b;

        if (linearly_dependent(a, b)) {
            b[0] += 1e-3f * a[0]; //jiggle jiggle
            b[1] -= 1e-3f * a[1];
            b = normalized(b);
        }

        float cos_theta = dot(a,b);
        float theta = std::acos(clamp(0.0f, cos_theta, 1.0f));

        //both point the same way
        if (theta < 1e-3f)
            return a;

        float t = std::min(max_angle, theta) / theta;

        if (cos_theta > 0.9995f) {
            return nlerp(a, b, t);
        } else {
            float theta_t = theta * t;

            vec2 c = normalized(b - a*cos_theta);
            return a*std::cos(theta_t) + c*std::sin(theta_t);
        }
    }

}
