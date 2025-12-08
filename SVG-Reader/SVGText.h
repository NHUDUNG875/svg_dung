#pragma once
#include "SVGElement.h"
#include "CustomPoint.h"

namespace tinyxml2 { class XMLElement; }

class SVGText : public SVGElement {
private:
	float font_size;
	CustomPoint start;
	std::wstring content;
public:
	SVGText();
	SVGText(float, const CustomPoint&, const std::wstring&);
	SVGText(const SVGText&);
	SVGText& operator=(const SVGText&);
	~SVGText() override = default;
	SVGElement* clone() const override;

	float getFontSize() const;
	void setFontSize(float);
	CustomPoint getStart() const;
	void setStart(const CustomPoint&);
	std::wstring getContent() const;
	void setContent(const std::wstring&);

	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};