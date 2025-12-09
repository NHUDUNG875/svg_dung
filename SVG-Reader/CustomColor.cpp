#include "CustomColor.h"
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <algorithm> 
#include <stdexcept> 

using namespace Gdiplus;
using namespace std;

CustomColor::CustomColor() {
    r = 0; 
    g = 0;
    b = 0;
}

CustomColor::CustomColor(int red, int green, int blue) {
    r = red;
    g = green;
    b = blue;
}

int CustomColor::clampCustomColorValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
// chỉnh sửa để đọc đc màu
CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
    std::string tempStr = rgbString;
    // bỏ khoảng trắng
    tempStr.erase(std::remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());

    // 1) Hỗ trợ tên màu cơ bản
    // chuyển về chữ thường để so sánh
    std::string lower = tempStr;
    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "red")    return CustomColor(255, 0, 0);
    if (lower == "green")  return CustomColor(0, 128, 0);   // SVG "green" chuẩn là #008000
    if (lower == "blue")   return CustomColor(0, 0, 255);
    if (lower == "black")  return CustomColor(0, 0, 0);
    if (lower == "white")  return CustomColor(255, 255, 255);
    if (lower == "yellow") return CustomColor(255, 255, 0);
    if (lower == "cyan" || lower == "aqua")   return CustomColor(0, 255, 255);
    if (lower == "magenta" || lower == "fuchsia") return CustomColor(255, 0, 255);
    if (lower == "gray" || lower == "grey")   return CustomColor(128, 128, 128);

    // 2) Hỗ trợ dạng rgb(r,g,b)
    if (tempStr.size() >= 4 && tempStr.substr(0, 4) == "rgb(") {

        size_t start = tempStr.find('(');
        size_t end = tempStr.find(')');

        if (start == std::string::npos || end == std::string::npos || end <= start) {
            return CustomColor(0, 0, 0);
        }

        std::string values = tempStr.substr(start + 1, end - start - 1);
        std::stringstream ss(values);
        std::string segment;
        int components[3] = { 0, 0, 0 };
        int i = 0;

        while (std::getline(ss, segment, ',') && i < 3) {
            try {
                if (!segment.empty()) {
                    components[i] = std::stoi(segment);
                }
            }
            catch (...) {
                components[i] = 0;
            }
            ++i;
        }

        return CustomColor(
            clampCustomColorValue(components[0]),
            clampCustomColorValue(components[1]),
            clampCustomColorValue(components[2])
        );
    }

    // 3) Không nhận ra định dạng → trả đen
    return CustomColor(0, 0, 0);
}

std::string CustomColor::fromCustomColorToString() const {
    std::stringstream ss;
    ss << "rgb(" << r << "," << g << "," << b << ")";
    return ss.str();
}

CustomColor::operator Gdiplus::Color() const {
    return Gdiplus::Color(255,
        static_cast<BYTE>(r),
        static_cast<BYTE>(g),
        static_cast<BYTE>(b));
}