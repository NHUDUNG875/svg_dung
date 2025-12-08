#include "SVGPolyshapeBase.h"
#include "tinyxml2.h"

SVGPolyshapeBase::SVGPolyshapeBase()
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(std::vector<CustomPoint>())
{
}
SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<CustomPoint>& points)
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(points)
{
}
SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other)
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    points(other.points)
{
}
SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        points = other.points;
    }
    return *this;
}
SVGPolyshapeBase::~SVGPolyshapeBase() {}
//SVGElement* SVGPolyshapeBase::clone() const {
//    return new SVGPolyshapeBase(*this);
//}

std::vector<CustomPoint> SVGPolyshapeBase::getPoints() const {
    return points;
}
void SVGPolyshapeBase::setPoints(const std::vector<CustomPoint>& points) {
    this->points = points;
}

void SVGPolyshapeBase::parse(tinyxml2::XMLElement* node) {
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
 void SVGPolyshapeBase::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
 }
// void SVGPolyshapeBase::transform(Matrix* m)