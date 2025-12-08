#pragma once
#include "SVGGroup.h"

class SVGParser;
class SVGRenderer;

class SVGDocument {
private:

    std::string fileName;
    SVGGroup* rootGroup;

public:

    SVGDocument(std::string);
    ~SVGDocument();

    SVGGroup* getRootGroup();
    void parseSVGImage(SVGParser&, SVGFactoryPattern&);
    void renderSVGImage(SVGRenderer&, Gdiplus::Graphics&);
};
