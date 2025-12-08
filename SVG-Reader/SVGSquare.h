#pragma once
#include "SVGRectangle.h"

class SVGSquare : public SVGRectangle {
public:
	SVGSquare();
	SVGSquare(const CustomPoint&, float);
	SVGSquare(const SVGSquare&);
	SVGSquare& operator=(const SVGSquare&);
	~SVGSquare();
	SVGElement* clone() const override;

	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};
