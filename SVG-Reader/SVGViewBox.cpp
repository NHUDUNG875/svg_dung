#include "SVGViewBox.h"
#include <sstream>
#include <algorithm>

SVGViewBox::SVGViewBox() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}

SVGViewBox::SVGViewBox(float x, float y, float w, float h)
    : x(x), y(y), width(w), height(h) {}

SVGViewBox SVGViewBox::parse(const std::string& str) {
    SVGViewBox vb;
    if (str.empty()) return vb;

    std::string s = str;
    // Thay thế dấu phẩy bằng dấu cách để đảm bảo đọc đúng
    std::replace(s.begin(), s.end(), ',', ' ');

    std::stringstream ss(s);
    // Chuẩn SVG: min-x, min-y, width, height
    ss >> vb.x >> vb.y >> vb.width >> vb.height;

    return vb;
}

bool SVGViewBox::isValid() const {
    return width > 0.0f && height > 0.0f;
}