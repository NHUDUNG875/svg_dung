#pragma once
#include "CustomColor.h"
#include "Stroke.h"
#include <string>

namespace Gdiplus {
    class Graphics;
    class Color;
}

class SVGStyle {
private:
    CustomColor fillColor;
    float fillOpacity;
    Stroke* stroke;
    std::string fillRule;
    int fontStyle;  // 0: Regular, 1: Bold, 2: Italic
    int textAnchor; // 0: Start (Left), 1: Middle, 2: End (Right) <--- QUAN TRỌNG

public:
    SVGStyle();
    SVGStyle(CustomColor, float, Stroke*);
    SVGStyle(const SVGStyle&);
    SVGStyle& operator=(const SVGStyle&);
    ~SVGStyle();

    CustomColor getFillColor() const;
    void setFillColor(const CustomColor&);

    float getFillOpacity() const;
    void setFillOpacity(const float&);

    Stroke* getStroke() const;
    void setStroke(Stroke*);

    std::string getFillRule() const { return fillRule; }
    void setFillRule(const std::string& rule) { fillRule = rule; }

    int getFontStyle() const { return fontStyle; }
    void setFontStyle(int style) { fontStyle = style; }

    // --- Getter/Setter cho Text Anchor ---
    int getTextAnchor() const { return textAnchor; }
    void setTextAnchor(int anchor) { textAnchor = anchor; }
    // -------------------------------------

    Gdiplus::Color getGdiFillColor() const;
    void parse(const std::string&);
};