#include "SVGPolyshapeBase.h"
#include "tinyxml2.h"
#include <sstream>
#include <algorithm>
#include <vector>

SVGPolyshapeBase::SVGPolyshapeBase()
    : SVGElement("polyshapebase", "", SVGStyle()), points() {}

SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<CustomPoint>& points)
    : SVGElement("polyshapebase", "", SVGStyle()), points(points) {}

SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other)
    : SVGElement(other), points(other.points) {}

SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) {
        SVGElement::operator=(other);
        points = other.points;
    }
    return *this;
}

SVGPolyshapeBase::~SVGPolyshapeBase() {}

std::vector<CustomPoint> SVGPolyshapeBase::getPoints() const {
    return points;
}
void SVGPolyshapeBase::setPoints(const std::vector<CustomPoint>& points) {
    this->points = points;
}

void SVGPolyshapeBase::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);

    const char* pointsStr = node->Attribute("points");
    if (!pointsStr) return;

    std::string s = pointsStr;
    // QUAN TRỌNG: Thay thế toàn bộ dấu phẩy thành dấu cách để xử lý đồng nhất
    std::replace(s.begin(), s.end(), ',', ' ');

    std::stringstream ss(s);
    float x, y;
    std::vector<CustomPoint> pts;

    // Đọc từng cặp số, tự động bỏ qua khoảng trắng thừa
    while (ss >> x >> y) {
        pts.emplace_back(x, y);
    }
    this->setPoints(pts);
}