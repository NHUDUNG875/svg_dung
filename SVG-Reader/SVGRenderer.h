#pragma once

// --- QUAN TRỌNG: Ngăn xung đột macro min/max ---
#define NOMINMAX 
#include <windows.h>
#include <gdiplus.h>
// ---------------------------------------------

#include <vector>
#include "SVGElement.h"

// Forward declaration
class SVGRectangle;
class SVGCircle;
class SVGEllipse;
class SVGLine;
class SVGPolygon;
class SVGPolyline;
class SVGText;
class SVGSquare;
class SVGGroup;
class SVGPath;

class SVGRenderer {
private:
    float zoom;
    float rotate;

public:
    SVGRenderer();

    void renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect);
    void renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle);
    void renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* el);
    void renderLine(Gdiplus::Graphics& g, const SVGLine* l);
    void renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* p);
    void renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* p);
    void renderSquare(Gdiplus::Graphics& g, const SVGSquare* sq);
    void renderText(Gdiplus::Graphics& g, const SVGText* text);

    void renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup);
    void renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup);
    void renderPath(Gdiplus::Graphics& g, const SVGPath* path);
};