#include "SVGTransform.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>

static std::vector<float> parseNumbers(const std::string& s) {
    std::vector<float> nums;
    const char* cursor = s.c_str();
    while (*cursor) {
        while (*cursor && (std::isspace((unsigned char)*cursor) || *cursor == ',')) cursor++;
        if (!*cursor) break;

        float sign = 1.0f;
        if (*cursor == '-') { sign = -1.0f; cursor++; }
        else if (*cursor == '+') { cursor++; }

        float res = 0.0f;
        bool has = false;
        while (std::isdigit((unsigned char)*cursor)) { res = res * 10.0f + (*cursor - '0'); cursor++; has = true; }
        if (*cursor == '.') {
            cursor++; float frac = 0.1f;
            while (std::isdigit((unsigned char)*cursor)) { res += (*cursor - '0') * frac; frac *= 0.1f; cursor++; has = true; }
        }
        if (has) nums.push_back(res * sign);
        else cursor++;
    }
    return nums;
}

SVGTransform SVGTransform::parse(const std::string& str) {
    SVGTransform t;
    std::string s = str;
    size_t pos = 0;
    while (pos < s.size()) {
        while (pos < s.size() && std::isspace((unsigned char)s[pos])) ++pos;
        if (pos >= s.size()) break;

        if (s.compare(pos, 9, "translate") == 0) {
            pos += 9;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
            float tx = nums.size() > 0 ? nums[0] : 0.0f;
            float ty = nums.size() > 1 ? nums[1] : 0.0f;
            t.addTranslate(tx, ty);
            pos = r + 1;
        }
        else if (s.compare(pos, 6, "rotate") == 0) {
            pos += 6;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
            float deg = nums.size() > 0 ? nums[0] : 0.0f;
            if (nums.size() >= 3) {
                float cx = nums[1], cy = nums[2];
                t.addTranslate(cx, cy); t.addRotate(deg); t.addTranslate(-cx, -cy);
            }
            else t.addRotate(deg);
            pos = r + 1;
        }
        else if (s.compare(pos, 5, "scale") == 0) {
            pos += 5;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
            float sx = (nums.size() > 0) ? nums[0] : 1.0f;
            float sy = (nums.size() > 1) ? nums[1] : sx;
            t.addScale(sx, sy);
            pos = r + 1;
        }
        else ++pos;
    }
    return t;
}

void SVGTransform::applyToMatrix(Gdiplus::Matrix& m) const {
    for (const auto& x : ops) {
        switch (x.type) {
        case TransformType::Translate: m.Translate(x.a, x.b, Gdiplus::MatrixOrderAppend); break;
        case TransformType::Rotate: m.Rotate(x.a, Gdiplus::MatrixOrderAppend); break;
        case TransformType::Scale: m.Scale(x.a, x.b, Gdiplus::MatrixOrderAppend); break;
        }
    }
}