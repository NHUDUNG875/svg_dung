#include "CustomPoint.h"
#include <cmath>

CustomPoint::CustomPoint() : x(0.0f), y(0.0f) {} // Dùng initializer list tốt hơn

CustomPoint::CustomPoint(float temp_x, float temp_y) : x(temp_x), y(temp_y) {}

// --- SỬA LỖI LOGIC TẠI ĐÂY ---
float CustomPoint::distanceTo(const CustomPoint& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}
// -----------------------------

CustomPoint::operator Gdiplus::PointF() const {
    return Gdiplus::PointF(x, y);
}

CustomPoint::operator Gdiplus::Point() const {
    return Gdiplus::Point(static_cast<INT>(x), static_cast<INT>(y));
}