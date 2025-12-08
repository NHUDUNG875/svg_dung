#include <windows.h>//
#include <gdiplus.h>//
#include "SVGRenderer.h"
#include "Stroke.h"        
#include "SVGRectangle.h"  
#include "SVGCircle.h"    
#include "SVGElement.h" 
#include "SVGGroup.h"//
#include "SVGTransform.h"//

namespace Gdiplus {
    struct PointF;
    class Graphics;
    class SolidBrush;
    class Pen;
    class Color;
}
using Gdiplus::PointF;
using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::Color;
SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}
//void SVGRenderer::setZoom(double z) {
//    zoom = static_cast<float>(z);
//}
//
//void SVGRenderer::setRotation(double deg) {
//    rotate = static_cast<float>(deg);
//}
//pair<double, double> SVGRenderer::transformPoint(double x, double y) const {
//    double rad = rotate * M_PI / 180.0;
//    double newX = (x * cos(rad) - y * sin(rad)) * zoom;
//    double newY = (x * sin(rad) + y * cos(rad)) * zoom;
//    return { newX, newY };
//}

void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;

    Gdiplus::Point p = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
    float w = rect->getWidth();
    float h = rect->getHeight();

    Gdiplus::SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);

    Stroke* strokeObj = rect->getSVGStyle().getStroke();
    if (strokeObj != nullptr) {
        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);

        g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
    }
}

void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;

    // get infor
    Gdiplus::PointF center = circle->getCenter();
    float radius = circle->getRadius();

    // get fill color
    Gdiplus::SolidBrush brush(circle->getSVGStyle().getGdiFillColor());

    // 1. fill the circle
    float diameter = radius * 2;
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);

    // 2. check and render stroke
    Stroke* strokeObj = circle->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;
        if (strokeW <= 0.0f) {
            strokeW = 1.0f; 
        }
        // create Pen
        Gdiplus::Pen pen(strokeColor, strokeW);

        // draw Stroke
        g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
    }
}

void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;

    PointF center = ellipse->getCenter();
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();


    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getGdiFillColor());
    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);


    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;

        Gdiplus::Pen pen(strokeColor, strokeW);
        g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    }
}
void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;

    PointF start = line->getStartPoint();
    PointF end = line->getEndPoint();


    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;

        Gdiplus::Pen pen(strokeColor, strokeW);
        g.DrawLine(&pen, start, end);
    }
}
void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;
    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
    std::vector<PointF> gdiPoints;
    gdiPoints.reserve(customPoints.size());
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    Gdiplus::SolidBrush brush(polygon->getSVGStyle().getGdiFillColor());
    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));

    Stroke* strokeObj = polygon->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
        g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }
}

void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;

    std::vector<PointF> gdiPoints;
    const auto& customPoints = polyline->getPoints();

    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }

    if (gdiPoints.size() < 2) return;

    Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();


    if (fillColor.GetA() > 0) {
        Gdiplus::SolidBrush brush(fillColor);
        g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }

    Stroke* strokeObj = polyline->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        if (strokeW <= 0.0f) {
            strokeW = 1.0f;
        }

        Gdiplus::Pen pen(finalStrokeColor, strokeW);
        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }
}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getStart();
    std::wstring content = text->getContent();
    Color color = text->getSVGStyle().getGdiFillColor();
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Times New Roman");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 1.0f);
    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
}

void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;
    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();
    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
    Gdiplus::Pen pen(square->getSVGStyle().getStroke()->strokeColor, square->getSVGStyle().getStroke()->strokeWidth);
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);
}
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;

    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix newMatrix = oldMatrix;
    rootGroup->getTransform().applyToMatrix(newMatrix);
    g.SetTransform(&newMatrix);

    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);

    //// Tạo matrix mới với identity
    //Gdiplus::Matrix newMatrix;
    //newMatrix.SetMatrix(
    //    oldMatrix.m11, oldMatrix.m12,
    //    oldMatrix.m21, oldMatrix.m22,
    //    oldMatrix.dx, oldMatrix.dy
    //);

    // Áp dụng transform của group lên newMatrix
    rootGroup->getTransform().applyToMatrix(newMatrix);
    g.SetTransform(&newMatrix);


    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        if(element)
            element->render(*this, g);
    }

    g.SetTransform(&oldMatrix);
}
