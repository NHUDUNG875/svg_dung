#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGTransform.h"  // th�m

class SVGRenderer;
namespace Gdiplus { class Graphics; }
namespace tinyxml2 { class XMLElement; }

class SVGElement {
protected:
	std::string tag_name;
	std::string id;
	SVGStyle style;
	SVGTransform transform;//th�m
public:
	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&) = default;
	SVGElement& operator=(const SVGElement&) = default;
	virtual ~SVGElement() = default;
	virtual SVGElement* clone() const = 0;

	std::string getTagName() const;
	void setTagName(const std::string&);
	std::string getId() const;
	void setId(const std::string&);
	SVGStyle& getSVGStyle();
	const SVGStyle& getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	//th�m
	const SVGTransform& getTransform() const { return transform; }
	SVGTransform& getTransform() { return transform; }
	void setTransform(const SVGTransform& t) { transform = t; }

	// polymorphism and separation of concerns
	virtual void parse(tinyxml2::XMLElement*) = 0;
	virtual void render(SVGRenderer&, Gdiplus::Graphics&) const = 0;
	// virtual void transform(Matrix* m) = 0;
};