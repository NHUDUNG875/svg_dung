#pragma once
#include "SVGElement.h"
#include <vector>
#include "CustomPoint.h" 

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute; 
	class XMLNode;
}
class SVGPolyshapeBase : public SVGElement {
protected:
	std::vector<CustomPoint> points;
public:
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<CustomPoint>&);
	SVGPolyshapeBase(const SVGPolyshapeBase&);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase&);
	~SVGPolyshapeBase();
	//SVGElement* clone() const;

	std::vector<CustomPoint> getPoints() const;
	void setPoints(const std::vector<CustomPoint>&);

	void parse(tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&) const = 0;
	// void transform(Matrix*) override;
};
