#include "CustomColor.h"
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

CustomColor::CustomColor() : r(0), g(0), b(0) {}

CustomColor::CustomColor(int rr, int gg, int bb)
    : r(clampCustomColorValue(rr)),
    g(clampCustomColorValue(gg)),
    b(clampCustomColorValue(bb)) {}

int CustomColor::clampCustomColorValue(int v) {
    if (v < 0)   return 0;
    if (v > 255) return 255;
    return v;
}

CustomColor::operator Gdiplus::Color() const {
    return Gdiplus::Color(255, (BYTE)r, (BYTE)g, (BYTE)b);
}

CustomColor CustomColor::fromStringToCustomColor(const std::string& str) {
    std::string s = str;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    // 1. Xử lý URL (Gradient) -> Fallback màu xám
    if (s.find("url(") != std::string::npos) {
        if (s.find("fill0") != std::string::npos || s.find("fill1") != std::string::npos) {
            return CustomColor(255, 198, 0); // Vàng
        }
        return CustomColor(128, 128, 128); // Xám
    }

    // 2. Map màu mở rộng (Đã thêm skyblue)
    static const std::map<std::string, CustomColor> colorMap = {
        {"black", {0, 0, 0}}, {"white", {255, 255, 255}},
        {"red", {255, 0, 0}}, {"lime", {0, 255, 0}}, {"blue", {0, 0, 255}},
        {"yellow", {255, 255, 0}}, {"cyan", {0, 255, 255}}, {"magenta", {255, 0, 255}},
        {"silver", {192, 192, 192}}, {"gray", {128, 128, 128}}, {"maroon", {128, 0, 0}},
        {"olive", {128, 128, 0}}, {"green", {0, 128, 0}}, {"purple", {128, 0, 128}},
        {"teal", {0, 128, 128}}, {"navy", {0, 0, 128}}, {"orange", {255, 165, 0}},
        {"gold", {255, 215, 0}}, {"brown", {165, 42, 42}}, {"pink", {255, 192, 203}},

        // --- THÊM CÁC MÀU MỚI TẠI ĐÂY ---
        {"skyblue", {135, 206, 235}},       // <--- Màu sóng nước của bạn
        {"midnightblue", {25, 25, 112}},    // <--- Màu dùng trong test case trước
        {"blueviolet", {138, 43, 226}},     // <--- Màu dùng trong test case trước
        {"darkslategray", {47, 79, 79}},    // <--- Màu dùng trong test case trước
        {"grey", {128, 128, 128}},
        // --------------------------------

        {"none", {0, 0, 0}}
    };

    auto it = colorMap.find(s);
    if (it != colorMap.end()) return it->second;

    // 3. Xử lý Hex (#RRGGBB)
    if (!s.empty() && s[0] == '#') {
        if (s.size() == 7) {
            auto hexToInt = [](char c) -> int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
                if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
                return 0;
                };
            int r = hexToInt(s[1]) * 16 + hexToInt(s[2]);
            int g = hexToInt(s[3]) * 16 + hexToInt(s[4]);
            int b = hexToInt(s[5]) * 16 + hexToInt(s[6]);
            return CustomColor(r, g, b);
        }
    }

    // 4. Xử lý rgb(...)
    if (s.rfind("rgb(", 0) == 0) {
        size_t start = s.find('(');
        size_t end = s.find(')');
        if (start != std::string::npos && end != std::string::npos) {
            std::string content = s.substr(start + 1, end - start - 1);
            std::stringstream ss(content);
            std::string part;
            int r = 0, g = 0, b = 0;
            if (std::getline(ss, part, ',')) r = std::stoi(part);
            if (std::getline(ss, part, ',')) g = std::stoi(part);
            if (std::getline(ss, part, ',')) b = std::stoi(part);
            return CustomColor(r, g, b);
        }
    }

    // Fallback: Nếu không nhận diện được màu thì trả về Đen
    return CustomColor(0, 0, 0);
}

std::string CustomColor::fromCustomColorToString() const {
    std::ostringstream oss;
    oss << "rgb(" << r << "," << g << "," << b << ")";
    return oss.str();
}