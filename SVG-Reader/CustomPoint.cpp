#include "CustomPoint.h"
#include <cmath>

CustomPoint::CustomPoint() {
    x = 0.0f;
    y = 0.0f;
}
CustomPoint::CustomPoint(float temp_x, float temp_y) {
    x = temp_x;
    y = temp_y;
};
float CustomPoint::distanceTo(const CustomPoint&) const {
    return std::sqrt(x * x + y * y);
}
CustomPoint::operator Gdiplus::PointF() const {
    return Gdiplus::PointF(x, y);
}
CustomPoint::operator Gdiplus::Point() const {
    return Gdiplus::Point(static_cast<int>(x), static_cast<int>(y));
}
// subsequent development
// CustomPoint& transform(Matrix* m);

