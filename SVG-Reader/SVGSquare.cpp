#include "SVGSquare.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGSquare::SVGSquare() : SVGRectangle(CustomPoint(0, 0), 0, 0) {
    this->setTagName("square");
}
SVGSquare::SVGSquare(const CustomPoint& topLeft, float sideLength) : SVGRectangle(topLeft, sideLength, sideLength) {
    this->setTagName("square");
}
SVGSquare::SVGSquare(const SVGSquare& other) : SVGRectangle(other) {
    this->setTagName("square");
}
SVGSquare& SVGSquare::operator=(const SVGSquare& other) {
    if (this != &other) {
        SVGRectangle::operator=(other);
    }
    return *this;
}
SVGSquare::~SVGSquare() {}
SVGElement* SVGSquare::clone() const {
    return new SVGSquare(*this);
}

void SVGSquare::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    // p.parseSquare(this, node);
    float x = node->FloatAttribute("x");
    float y = node->FloatAttribute("y");
    float size = node->FloatAttribute("width"); 
    this->setTopLeftCorner(CustomPoint(x, y));
    this->setWidth(size);
    this->setHeight(size);
}
void SVGSquare::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderSquare(g, this);
}
// void SVGSquare::transform(Matrix* m)