#pragma once
#include "SVGElement.h"
#include "CustomPoint.h"

namespace tinyxml2 { class XMLElement; }

class SVGEllipse : public SVGElement {
protected:
	CustomPoint center;
	float rx, ry;
public:
	SVGEllipse();
	SVGEllipse(CustomPoint, float, float);
	SVGEllipse(const SVGEllipse&);
	SVGEllipse& operator=(const SVGEllipse&);
	~SVGEllipse();
	SVGElement* clone() const override;

	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadiusX() const;
	void setRadiusX(const float&);
	float getRadiusY() const;
	void setRadiusY(const float&);

	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};
