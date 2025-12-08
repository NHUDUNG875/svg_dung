#include "SVGStyle.h"
#include "Stroke.h" 
#include "CustomColor.h" 
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

SVGStyle::SVGStyle() : fillOpacity(1.0f), stroke(nullptr) {
}

SVGStyle::SVGStyle(CustomColor fillC, float fillO, Stroke* s)
    : fillColor(fillC), fillOpacity(fillO), stroke(s ? new Stroke(*s) : nullptr) {
}
SVGStyle::SVGStyle(const SVGStyle& other) {
    this->fillColor = other.fillColor;
    this->fillOpacity = other.fillOpacity;
    this->stroke = other.stroke ? new Stroke(*other.stroke) : nullptr; 
}
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        delete this->stroke;
        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;
        this->stroke = other.stroke ? new Stroke(*other.stroke) : nullptr;
    }
    return *this;
}
SVGStyle::~SVGStyle() {
    delete stroke;
}
CustomColor SVGStyle::getFillColor() const {
    return fillColor;
}
void SVGStyle::setFillColor(const CustomColor& color) {
    fillColor = color;
}
float SVGStyle::getFillOpacity() const {
    return fillOpacity;

}
void SVGStyle::setFillOpacity(const float& opacity) {
    fillOpacity = opacity;
}
Stroke* SVGStyle::getStroke() const {
    return stroke;
}
void SVGStyle::setStroke(Stroke* newStroke) {
    if (stroke != nullptr) {
        delete stroke;
        stroke = nullptr; 
    }

    if (newStroke != nullptr) {
        stroke = new Stroke(*newStroke);
    }
}
Gdiplus::Color SVGStyle::getGdiFillColor() const {
    BYTE alpha = static_cast<BYTE>(std::round(fillOpacity * 255.0f));

    Gdiplus::Color baseColor = fillColor;

    return Gdiplus::Color(
        alpha,
        baseColor.GetR(),
        baseColor.GetG(),
        baseColor.GetB()
    );
}
void SVGStyle::parse(const std::string& styleStr) {
    std::stringstream ss(styleStr);
    std::string item;
    while (getline(ss, item, ';')) {
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;

        size_t pos = item.find(':');
        if (pos == std::string::npos) continue;

        std::string key = item.substr(0, pos);
        std::string value = item.substr(pos + 1);

        if (key == "fill") this->setFillColor(CustomColor::fromStringToCustomColor(value));
        else if (key == "fill-opacity") this->setFillOpacity(stof(value));
        else if (key.find("stroke") != std::string::npos) {
            Stroke* stroke = this->getStroke();
            std::string s = styleStr;
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
            size_t pos = s.find(':');
            if (pos == std::string::npos) return;
            std::string key = s.substr(0, pos);
            std::string value = s.substr(pos + 1);
            if (key == "stroke") stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
            else if (key == "stroke-width") stroke->strokeWidth = stof(value);
            else if (key == "stroke-opacity") stroke->strokeOpacity = stof(value);

            this->setStroke(stroke);
        }
    }
}
// void SVGStyle::render(SVGRenderer& r) const {
//     r.renderStyle(*this);
// }  
// void transform(Matrix*);         