#pragma once
#include "SVGGroup.h"
#include "SVGFactoryPattern.h"
#include <algorithm>
#include "tinyxml2.h" 
#include "SVGRenderer.h"

class SVGGroup : public SVGElement {
private:

    std::vector<SVGElement*> ElementArray;
    SVGGroup* parent;

    float width = 0.0f;     
    float height = 0.0f;   
    std::string viewBox;  
   
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

    void parse(tinyxml2::XMLElement*);
    void render(SVGRenderer&, Gdiplus::Graphics&) const;

    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
    void setViewBox(const std::string& vb) { viewBox = vb; }

    void printDebugAttributes(std::ostream& os) const;
};

