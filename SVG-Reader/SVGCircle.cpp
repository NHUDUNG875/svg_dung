#include "SVGCircle.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGCircle::SVGCircle() : SVGEllipse() {
    this->setTagName("circle");
}
SVGCircle::SVGCircle(CustomPoint center, float radius) : SVGEllipse(center, radius, radius) {
    this->setTagName("circle");
}
SVGCircle::SVGCircle(const SVGCircle& other) : SVGEllipse(other) {
    this->setTagName("circle");
}
SVGCircle& SVGCircle::operator=(const SVGCircle& other) {
    if (this != &other) {
        this->setCenter(other.getCenter());
        this->setRadius(other.getRadius());
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}
SVGCircle::~SVGCircle() {}
SVGElement* SVGCircle::clone() const {
    return new SVGCircle(*this);
}

CustomPoint SVGCircle::getCenter() const {
    return this->center;
}
void SVGCircle::setCenter(const CustomPoint& center) {
    this->center = center;
}
float SVGCircle::getRadius() const {
    return this->rx;
}
void SVGCircle::setRadius(const float& radius) {
    this->rx = radius;
    this->ry = radius;
}

void SVGCircle::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    // p.parseCircle(this, node);
    float cx = node->FloatAttribute("cx");
    float cy = node->FloatAttribute("cy");
    float r = node->FloatAttribute("r");
    this->setCenter(CustomPoint(cx, cy));
    this->setRadius(r);
}
void SVGCircle::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderCircle(g, this);
}
// void SVGCircle::transform(Matrix* m)
