#include "SVGStyle.h"
// Include Windows/GDI+ trước để tránh xung đột
#include <windows.h>
#include <gdiplus.h>

#include "Stroke.h" 
#include "CustomColor.h" 
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace Gdiplus;

// Constructor mặc định (Gộp chung logic khởi tạo)
SVGStyle::SVGStyle()
    : fillColor(0, 0, 0), // Mặc định đen
    fillOpacity(1.0f),
    stroke(nullptr),
    fillRule("nonzero"),
    fontStyle(0)
{
}

// Constructor có tham số
SVGStyle::SVGStyle(CustomColor fillC, float fillO, Stroke* s)
    : fillColor(fillC),
    fillOpacity(fillO),
    stroke(s ? new Stroke(*s) : nullptr),
    fillRule("nonzero"),
    fontStyle(0)
{
}

// Copy Constructor
SVGStyle::SVGStyle(const SVGStyle& other)
    : fillColor(other.fillColor),
    fillOpacity(other.fillOpacity),
    fillRule(other.fillRule),
    fontStyle(other.fontStyle),
    stroke(other.stroke ? new Stroke(*other.stroke) : nullptr)
{
}

// Copy Assignment Operator
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        // Xóa stroke cũ trước khi copy mới để tránh leak memory
        delete this->stroke;

        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;
        this->fillRule = other.fillRule;
        this->fontStyle = other.fontStyle;
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
    // Ép kiểu BYTE (unsigned char) của Windows
    BYTE alpha = static_cast<BYTE>(std::round(fillOpacity * 255.0f));
    Gdiplus::Color baseColor = fillColor;
    return Gdiplus::Color(alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());
}

void SVGStyle::parse(const std::string& styleStr) {
    std::stringstream ss(styleStr);
    std::string item;

    while (std::getline(ss, item, ';')) {
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;

        size_t pos = item.find(':');
        if (pos == std::string::npos) continue;

        std::string key = item.substr(0, pos);
        std::string value = item.substr(pos + 1);

        if (key == "fill") {
            this->setFillColor(CustomColor::fromStringToCustomColor(value));
        }
        else if (key == "fill-opacity") {
            try { this->setFillOpacity(std::stof(value)); }
            catch (...) {}
        }
        else if (key == "fill-rule") {
            this->setFillRule(value);
        }
        else if (key == "font-style" && value == "italic") {
            this->setFontStyle(this->getFontStyle() | 2); // 2 = Italic
        }
        else if (key == "font-weight" && (value == "bold" || value == "700")) {
            this->setFontStyle(this->getFontStyle() | 1); // 1 = Bold
        }
        else if (key.find("stroke") != std::string::npos) {
            Stroke* s = this->getStroke();
            if (!s) {
                Stroke tmp;
                this->setStroke(&tmp);
                s = this->getStroke();
            }
            if (!s) continue;

            if (key == "stroke") s->strokeColor = CustomColor::fromStringToCustomColor(value);
            else if (key == "stroke-width") try { s->strokeWidth = std::stof(value); }
            catch (...) {}
            else if (key == "stroke-opacity") try { s->strokeOpacity = std::stof(value); }
            catch (...) {}
            else if (key == "stroke-miterlimit") try { s->miterLimit = std::stof(value); }
            catch (...) {}
        }
    }
}