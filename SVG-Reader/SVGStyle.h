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
	std::string fillRule; // Thêm std::
	int fontStyle;

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

	// Thêm getter/setter cho fillRule
	std::string getFillRule() const { return fillRule; }
	void setFillRule(const std::string& rule) { fillRule = rule; }

	// Thêm getter/setter cho fontStyle
	int getFontStyle() const { return fontStyle; }
	void setFontStyle(int style) { fontStyle = style; }

	Gdiplus::Color getGdiFillColor() const;

	void parse(const std::string&); // Thêm std::
};