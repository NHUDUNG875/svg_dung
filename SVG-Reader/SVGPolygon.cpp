#include "SVGPolygon.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGPolygon::SVGPolygon() : SVGPolyshapeBase() {
    this->setTagName("polygon");
}
SVGPolygon::SVGPolygon(const std::vector<CustomPoint>& points) : SVGPolyshapeBase(points) {
    this->setTagName("polygon");
}
SVGPolygon::SVGPolygon(const SVGPolygon& other) : SVGPolyshapeBase(other.points) {
    this->setTagName("polygon");
    this->setId(other.getId());
    this->setSVGStyle(other.getSVGStyle());
}
SVGPolygon& SVGPolygon::operator=(const SVGPolygon& other) {
    if (this != &other) {
        this->setPoints(other.getPoints());

        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}
SVGPolygon::~SVGPolygon() {}
SVGElement* SVGPolygon::clone() const {
    return new SVGPolygon(*this);
}

void SVGPolygon::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    // p.parsePolyshape(this, node);
    const char* pointsStr = node->Attribute("points");
    if (!pointsStr) return;

    std::vector<CustomPoint> pts;
    std::stringstream ss(pointsStr);
    std::string token;
    while (getline(ss, token, ' ')) {
        if (token.empty()) continue;
        size_t comma = token.find(',');
        if (comma == std::string::npos) continue;
        float x = stof(token.substr(0, comma));
        float y = stof(token.substr(comma + 1));
        pts.emplace_back(x, y);
    }
    this->setPoints(pts);
}
void SVGPolygon::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderPolygon(g, this);
}
// void SVGPolygon::transform(Matrix* m)
