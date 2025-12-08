#include "SVGEllipse.h"
#include "tinyxml2.h" 
#include "SVGRenderer.h"

SVGEllipse::SVGEllipse()
    : SVGElement("ellipse", "", SVGStyle()),
    center(0, 0),
    rx(0.0f),
    ry(0.0f)
{
}
SVGEllipse::SVGEllipse(CustomPoint center, float rx, float ry)
    : SVGElement("ellipse", "", SVGStyle()),
    center(center),
    rx(rx),
    ry(ry)
{
}
SVGEllipse::SVGEllipse(const SVGEllipse& other) {
    this->setTagName(other.getTagName());
    this->setId(other.getId());
    this->setSVGStyle(other.getSVGStyle());
    this->center = other.center;
    this->rx = other.rx;
    this->ry = other.ry;
}
SVGEllipse& SVGEllipse::operator=(const SVGEllipse& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        this->center = other.center;
        this->rx = other.rx;
        this->ry = other.ry;
    }
    return (*this);
}
SVGEllipse::~SVGEllipse() {}
SVGElement* SVGEllipse::clone() const {
    return new SVGEllipse(*this);
}

CustomPoint SVGEllipse::getCenter() const {
    return center;
}
void SVGEllipse::setCenter(const CustomPoint& center) {
    this->center = center;
}
float SVGEllipse::getRadiusX() const {
    return rx;
}
void SVGEllipse::setRadiusX(const float& rx) {
    this->rx = rx;
}
float SVGEllipse::getRadiusY() const {
    return ry;
}
void SVGEllipse::setRadiusY(const float& ry) {
    this->ry = ry;
}

void SVGEllipse::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    // p.parseEllipse(this, node);
    float cx = node->FloatAttribute("cx");
    float cy = node->FloatAttribute("cy");
    float rx = node->FloatAttribute("rx");
    float ry = node->FloatAttribute("ry");
    this->setCenter(CustomPoint(cx, cy));
    this->setRadiusX(rx);
    this->setRadiusY(ry);
}
void SVGEllipse::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderEllipse(g, this);
}
// void SVGEllipse::transform(Matrix* m)