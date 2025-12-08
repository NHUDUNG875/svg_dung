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

CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
    string tempStr = rgbString;
    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());

    if (tempStr.substr(0, 4) != "rgb(") {
        return CustomColor(0, 0, 0);
    }

    size_t start = tempStr.find('(');
    size_t end = tempStr.find(')');

    if (start == string::npos || end == string::npos || end <= start) {
        return CustomColor(0, 0, 0);
    }

    string values = tempStr.substr(start + 1, end - start - 1);
    stringstream ss(values);
    string segment;
    int components[3] = { 0, 0, 0 };
    int i = 0;

    while (getline(ss, segment, ',') && i < 3) {
        try {
            if (!segment.empty()) {
                components[i] = std::stoi(segment);
            }
        }
        catch (const std::exception& e) {
            components[i] = 0;
        }
        i++;
    }

    return CustomColor(
        clampCustomColorValue(components[0]),
        clampCustomColorValue(components[1]),
        clampCustomColorValue(components[2])
    );
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