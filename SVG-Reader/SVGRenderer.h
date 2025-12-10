#pragma once
#include <gdiplus.h>
#include "SVGRectangle.h"
#include "SVGCircle.h"
#include "SVGEllipse.h" 
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGText.h"
#include "SVGSquare.h"
#include "SVGPath.h"

// QUAN TRỌNG: Forward declaration để tránh include chéo
class SVGGroup;

class SVGRenderer {
private:
    float zoom;
    float rotate;

public:
    SVGRenderer();
    ~SVGRenderer() = default;

    void renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect);
    void renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle);
    void renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse);
    void renderLine(Gdiplus::Graphics& g, const SVGLine* line);
    void renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon);
    void renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline);
    void renderText(Gdiplus::Graphics& g, const SVGText* text);
    void renderSquare(Gdiplus::Graphics& g, const SVGSquare* square);

    // Hàm renderGroup cần SVGGroup, nhưng ở .h chỉ cần class declaration
    void renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup);

    void renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup);
    void renderPath(Gdiplus::Graphics& g, const SVGPath* path);
};