#pragma once
#include "SVGGroup.h"
#include <string>

class SVGParser;
class SVGRenderer;
class SVGFactoryPattern;

class SVGDocument {
private:
    std::string fileName;
    SVGGroup* rootGroup;
public:
    SVGDocument(std::string);
    ~SVGDocument();
    SVGGroup* getRootGroup();
    void parseSVGImage(SVGParser&, SVGFactoryPattern&); // Tham số Factory
    void renderSVGImage(SVGRenderer&, Gdiplus::Graphics&);
    SVGViewBox getViewBox() const;
    float getWidth() const;
    float getHeight() const;
};