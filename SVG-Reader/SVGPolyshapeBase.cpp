#include "SVGPolyshapeBase.h"
#include "tinyxml2.h"
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

// --- FIX: Hàm đọc số bất chấp Locale (Dấu chấm hay phẩy đều chạy tốt) ---
static bool parseNextFloat(const char*& cursor, float& outVal) {
    // 1. Bỏ qua rác (khoảng trắng, dấu phẩy)
    while (*cursor && (std::isspace((unsigned char)*cursor) || *cursor == ',')) {
        cursor++;
    }
    if (!*cursor) return false;

    // 2. Xử lý dấu
    float sign = 1.0f;
    if (*cursor == '-') { sign = -1.0f; cursor++; }
    else if (*cursor == '+') { cursor++; }

    // 3. Đọc phần nguyên
    float result = 0.0f;
    bool hasDigits = false;
    while (std::isdigit((unsigned char)*cursor)) {
        result = result * 10.0f + (*cursor - '0');
        cursor++;
        hasDigits = true;
    }

    // 4. Đọc phần thập phân (BẮT BUỘC DÙNG DẤU CHẤM CỦA SVG)
    if (*cursor == '.') {
        cursor++;
        float fraction = 0.1f;
        while (std::isdigit((unsigned char)*cursor)) {
            result += (*cursor - '0') * fraction;
            fraction *= 0.1f;
            cursor++;
            hasDigits = true;
        }
    }

    // 5. Đọc số mũ (Scientific notation: 1.2e-5)
    if (hasDigits && (*cursor == 'e' || *cursor == 'E')) {
        const char* temp = cursor + 1;
        float expSign = 1.0f;
        if (*temp == '-') { expSign = -1.0f; temp++; }
        else if (*temp == '+') temp++;

        if (std::isdigit((unsigned char)*temp)) {
            cursor = temp;
            int exponent = 0;
            while (std::isdigit((unsigned char)*cursor)) {
                exponent = exponent * 10 + (*cursor - '0');
                cursor++;
            }
            result = result * std::pow(10.0f, exponent * expSign);
        }
    }

    if (!hasDigits) return false;

    outVal = result * sign;
    return true;
}

SVGPolyshapeBase::SVGPolyshapeBase() : SVGElement("polyshapebase", "", SVGStyle()), points() {}
SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<CustomPoint>& points) : SVGElement("polyshapebase", "", SVGStyle()), points(points) {}
SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other) : SVGElement(other), points(other.points) {}
SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) { SVGElement::operator=(other); points = other.points; }
    return *this;
}
SVGPolyshapeBase::~SVGPolyshapeBase() {}

std::vector<CustomPoint> SVGPolyshapeBase::getPoints() const { return points; }
void SVGPolyshapeBase::setPoints(const std::vector<CustomPoint>& points) { this->points = points; }

void SVGPolyshapeBase::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    const char* pointsStr = node->Attribute("points");
    if (!pointsStr) return;

    std::vector<CustomPoint> pts;
    float x, y;
    const char* cursor = pointsStr;

    // Đọc liên tục x, y
    while (parseNextFloat(cursor, x)) {
        if (parseNextFloat(cursor, y)) {
            pts.emplace_back(x, y);
        }
        else break;
    }
    this->setPoints(pts);
}