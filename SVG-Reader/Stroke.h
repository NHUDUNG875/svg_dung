#pragma once
#include "CustomColor.h"

namespace Gdiplus {
	class Color;
}

struct Stroke {
public:
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;
	float miterLimit;

	Stroke();
	Stroke(CustomColor, float, float, float miter = 4.0f);
	Stroke(CustomColor, float, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();

	// there is no getter/setter because all attributes are public

	Gdiplus::Color getGdiColor() const;

	void parse(const std::string&);
	// void render(SVGRenderer&) const;
	// void transform(Matrix*);
};
