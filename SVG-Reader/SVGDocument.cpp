#include "SVGDocument.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGFactoryPattern.h"

SVGDocument::SVGDocument(std::string fileInput) { this->fileName = fileInput; this->rootGroup = nullptr; }
SVGDocument::~SVGDocument() { delete rootGroup; rootGroup = nullptr; }
SVGGroup* SVGDocument::getRootGroup() { return this->rootGroup; }
void SVGDocument::parseSVGImage(SVGParser& parser, SVGFactoryPattern& factory) {
    this->rootGroup = parser.readXML(this->fileName, factory);
}
void SVGDocument::renderSVGImage(SVGRenderer& render, Gdiplus::Graphics& graphics) {
    render.renderFigure(graphics, this->rootGroup);
}
SVGViewBox SVGDocument::getViewBox() const {
    return (rootGroup) ? rootGroup->getViewBox() : SVGViewBox();
}
float SVGDocument::getWidth() const {
    return (rootGroup) ? rootGroup->getWidth() : 0.0f;
}
float SVGDocument::getHeight() const {
    return (rootGroup) ? rootGroup->getHeight() : 0.0f;
}