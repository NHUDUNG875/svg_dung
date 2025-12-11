#pragma once
#include <string>
#include <algorithm>
#include <map>

namespace Gdiplus {
    class Color;
    typedef unsigned char BYTE;
}

struct CustomColor {
    int r, g, b;
    CustomColor();
    CustomColor(int, int, int);

    static int clampCustomColorValue(int);
    static CustomColor fromStringToCustomColor(const std::string&);
    std::string fromCustomColorToString() const;

    operator Gdiplus::Color() const;
};