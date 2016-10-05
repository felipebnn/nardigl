#pragma once

namespace ns {

    template <class T>
    struct Rect {
        T x, y;
        T w, h;
    };

    typedef Rect<int> IntRect;
    typedef Rect<float> FloatRect;

}
