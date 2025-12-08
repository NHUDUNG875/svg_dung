#pragma once
#include "CustomPoint.h"
#include "SVGElement.h"

class SVGLine : public SVGElement {
private:
	CustomPoint start_point, end_point;
public:
	SVGLine();
	SVGLine(CustomPoint, CustomPoint);
	SVGLine(const SVGLine&);
	SVGLine& operator=(const SVGLine&);
	~SVGLine();
	SVGElement* clone() const override;

	CustomPoint getStartPoint() const;
	void setStartPoint(const CustomPoint&);
	CustomPoint getEndPoint() const;
	void setEndPoint(const CustomPoint&);

	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};
