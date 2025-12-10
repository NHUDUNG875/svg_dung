#pragma once
#include "SVGElement.h"
#include <vector>
#include "tinyxml2.h"
#include <algorithm>
#include "SVGViewBox.h" // Đảm bảo bạn đã có file này

class SVGRenderer;

class SVGGroup : public SVGElement {
private:
    std::vector<SVGElement*> ElementArray;
    SVGGroup* parent;
    float width = 0.0f;
    float height = 0.0f;

    SVGViewBox viewBox;

public:
    SVGGroup();
    SVGGroup(const SVGGroup&);
    SVGGroup& operator= (SVGGroup) noexcept;
    ~SVGGroup();

    friend void swap(SVGGroup& first, SVGGroup& second) noexcept;

    void setParent(SVGGroup*);
    void setElementArray(const std::vector<SVGElement*>&);
    void addElement(SVGElement*);

    SVGGroup* getParent();
    const std::vector<SVGElement*>& getSVGElementArray() const;

    SVGElement* clone() const override;

    void parse(tinyxml2::XMLElement*) override;
    void render(SVGRenderer&, Gdiplus::Graphics&) const override;

    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }

    void setViewBox(const SVGViewBox& vb) { viewBox = vb; }
    SVGViewBox getViewBox() const { return viewBox; }

    float getWidth() const { return width; }
    float getHeight() const { return height; }

    void printDebugAttributes(std::ostream& os) const;
};