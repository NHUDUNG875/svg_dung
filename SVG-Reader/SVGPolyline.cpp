#include "SVGPolyline.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGPolyline::SVGPolyline() : SVGPolyshapeBase() {
    this->setTagName("polyline");
}
SVGPolyline::SVGPolyline(const std::vector<CustomPoint>& points) : SVGPolyshapeBase(points) {
    this->setTagName("polyline");
}
SVGPolyline::SVGPolyline(const SVGPolyline& other) : SVGPolyshapeBase(other.getPoints()) {
    this->setTagName("polyline");
}
SVGPolyline& SVGPolyline::operator=(const SVGPolyline& other) {
    if (this != &other) {
        this->setPoints(other.getPoints());

        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}
SVGPolyline::~SVGPolyline() {}
SVGElement* SVGPolyline::clone() const {
    return new SVGPolyline(*this);
}

void SVGPolyline::parse(tinyxml2::XMLElement* node) {
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
void SVGPolyline::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderPolyline(g, this);
}
// void SVGPolyline::transform(Matrix* m)
