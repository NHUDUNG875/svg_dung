#pragma once
#include "SVGEllipse.h"

class SVGRenderer;
namespace Gdiplus { class Graphics; }

class SVGCircle : public SVGEllipse {
public:
	SVGCircle();
	SVGCircle(CustomPoint, float);
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();
	SVGElement* clone() const override;

	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadius() const;
	void setRadius(const float&);

	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};