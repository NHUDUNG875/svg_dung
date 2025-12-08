
#include "Stroke.h"
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

Stroke::Stroke() {
    this->strokeColor = CustomColor(0, 0, 0); 
    this->strokeOpacity = 1.0f;              
    this->strokeWidth = 1.0f;                
}
Stroke::Stroke(CustomColor color, float opacity, float width) {
    this->strokeColor = color;
    this->strokeOpacity = opacity;
    this->strokeWidth = width;
}

Stroke::Stroke(const Stroke& other)
    : strokeColor(other.strokeColor),  
    strokeOpacity(other.strokeOpacity), 
    strokeWidth(other.strokeWidth)      
{}

Stroke& Stroke::operator=(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
    }
    return (*this);
}
Stroke::~Stroke() {}

// there is no getter/setter because all attributes are public
Gdiplus::Color Stroke::getGdiColor() const {
    BYTE alpha = static_cast<BYTE>(strokeOpacity * 255.0f);

    Gdiplus::Color baseColor = strokeColor; 

    return Gdiplus::Color(alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());
}
void Stroke::parse(const std::string& strokeStr) {
    // p.parseStroke(*this, strokeStr);
    std::string s = strokeStr;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t pos = s.find(':');
    if (pos == std::string::npos) return;

    std::string key = s.substr(0, pos);
    std::string value = s.substr(pos + 1);

    if (key == "stroke") this->strokeColor = CustomColor::fromStringToCustomColor(value);
    else if (key == "stroke-width") this->strokeWidth = stof(value);
    else if (key == "stroke-opacity") this->strokeOpacity = stof(value);
}
// void STroke::render(SVGRenderer& r) const {
//     r.renderStroke();
// }
// void transform(Matrix*);

