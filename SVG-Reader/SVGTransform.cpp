#include "SVGTransform.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>

// Helper: Parse dãy số từ chuỗi
static std::vector<float> parseNumbers(const std::string& s)
{
    std::vector<float> nums;
    std::string cur;
    for (char ch : s) {
        if (ch == ',' || std::isspace((unsigned char)ch)) {
            if (!cur.empty()) {
                try { nums.push_back(std::stof(cur)); }
                catch (...) {}
                cur.clear();
            }
        }
        else {
            cur.push_back(ch);
        }
    }
    if (!cur.empty()) {
        try { nums.push_back(std::stof(cur)); }
        catch (...) {}
    }
    return nums;
}

SVGTransform SVGTransform::parse(const std::string& str)
{
    SVGTransform t;
    std::string s = str;

    // Xóa khoảng trắng đầu cuối
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());

    size_t pos = 0;
    while (pos < s.size()) {
        while (pos < s.size() && std::isspace((unsigned char)s[pos])) ++pos;
        if (pos >= s.size()) break;

        // 1. TRANSLATE
        if (s.compare(pos, 9, "translate") == 0) {
            pos += 9;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);
            float tx = nums.size() > 0 ? nums[0] : 0.0f;
            float ty = nums.size() > 1 ? nums[1] : 0.0f;
            t.addTranslate(tx, ty);
            pos = r + 1;
        }
        // 2. ROTATE
        else if (s.compare(pos, 6, "rotate") == 0) {
            pos += 6;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);
            float deg = nums.size() > 0 ? nums[0] : 0.0f;
            if (nums.size() >= 3) { // Rotate quanh điểm neo (cx, cy)
                float cx = nums[1]; float cy = nums[2];
                t.addTranslate(cx, cy);
                t.addRotate(deg);
                t.addTranslate(-cx, -cy);
            }
            else {
                t.addRotate(deg);
            }
            pos = r + 1;
        }
        // 3. SCALE
        else if (s.compare(pos, 5, "scale") == 0) {
            pos += 5;
            size_t l = s.find('(', pos); size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;
            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);

            float sx = 1.0f, sy = 1.0f;
            if (nums.size() == 1) {
                sx = nums[0];
                sy = nums[0];
            }
            else if (nums.size() >= 2) {
                sx = nums[0];
                sy = nums[1];
            }
            t.addScale(sx, sy);
            pos = r + 1;
        }
        else {
            ++pos;
        }
    }
    return t;
}

void SVGTransform::applyToMatrix(Gdiplus::Matrix& m) const
{
    // SVG áp dụng transform từ phải sang trái.
    // Nhưng Gdiplus::Matrix mặc định là Prepend (nhân vào bên trái).
    // Do đó, duyệt xuôi (0 -> end) với Prepend là đúng chuẩn SVG.
    for (const auto& x : ops) {
        switch (x.type)
        {
        case TransformType::Translate:
            m.Translate(x.a, x.b);
            break;
        case TransformType::Rotate:
            m.Rotate(x.a);
            break;
        case TransformType::Scale:
            m.Scale(x.a, x.b);
            break;
        }
    }
}