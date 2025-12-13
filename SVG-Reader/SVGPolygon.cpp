#define NOMINMAX 
#include "SVGPolygon.h"
#include <windows.h>
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGPolygon::SVGPolygon() : SVGPolyshapeBase() { this->setTagName("polygon"); }
SVGPolygon::SVGPolygon(const std::vector<CustomPoint>& points) : SVGPolyshapeBase(points) { this->setTagName("polygon"); }
SVGPolygon::SVGPolygon(const SVGPolygon& other) : SVGPolyshapeBase(other) { this->setTagName("polygon"); }
SVGPolygon& SVGPolygon::operator=(const SVGPolygon& other) {
    if (this != &other) { SVGPolyshapeBase::operator=(other); this->setTagName("polygon"); }
    return (*this);
}
SVGPolygon::~SVGPolygon() {}
SVGElement* SVGPolygon::clone() const { return new SVGPolygon(*this); }

void SVGPolygon::parse(tinyxml2::XMLElement* node) {
    // Gọi Parser xịn của lớp cha để đọc được dấu chấm/phẩy chính xác
    SVGPolyshapeBase::parse(node);
}

void SVGPolygon::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderPolygon(g, this);
}