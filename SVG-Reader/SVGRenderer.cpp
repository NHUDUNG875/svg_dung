#include <windows.h>//
#include <gdiplus.h>//
#include "SVGRenderer.h"
#include "Stroke.h"        
#include "SVGRectangle.h"  
#include "SVGCircle.h"    
#include "SVGElement.h" 
#include "SVGGroup.h"//
#include "SVGTransform.h"//
#include "SVGPath.h"

//using namespace  Gdiplus;

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

    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    rect->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    circle->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;

    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    ellipse->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;

    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    line->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    PointF start = line->getStartPoint();
    PointF end = line->getEndPoint();


    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;

        Gdiplus::Pen pen(strokeColor, strokeW);
        g.DrawLine(&pen, start, end);
    }
    g.SetTransform(&oldMatrix);

}
void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    polygon->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;

    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    polyline->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    text->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    PointF pos = text->getStart();
    std::wstring content = text->getContent();
    Color color = text->getSVGStyle().getGdiFillColor();
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Times New Roman");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 1.0f);
    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
    
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix localMatrix;
    square->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();
    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
    Gdiplus::Pen pen(square->getSVGStyle().getStroke()->strokeColor, square->getSVGStyle().getStroke()->strokeWidth);
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
    
    g.SetTransform(&oldMatrix);
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);
}

void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;

    ơ:Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Gdiplus::Matrix groupMatrix; 
    rootGroup->getTransform().applyToMatrix(groupMatrix);

    g.MultiplyTransform(&groupMatrix);


    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        if (element) {
            element->render(*this, g);
        }
    }

    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
    if (!path) return;

    Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    Matrix localMatrix;
    path->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    Gdiplus::GraphicsPath gp;
    Gdiplus::PointF current(0.0f, 0.0f);
    Gdiplus::PointF start(0.0f, 0.0f);

    for (const auto& cmd : path->getCommands()) {
        switch (cmd.type) {
        case PathCommandType::MoveTo: {
            float x = cmd.params[0], y = cmd.params[1];
            current = Gdiplus::PointF(x, y);
            start = current;
            break;
        }
        case PathCommandType::LineTo: {
            float x = cmd.params[0], y = cmd.params[1];
            Gdiplus::PointF p(x, y);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::HLineTo: {
            float x = cmd.params[0];
            Gdiplus::PointF p(x, current.Y);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::VLineTo: {
            float y = cmd.params[0];
            Gdiplus::PointF p(current.X, y);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::ClosePath: {
            gp.AddLine(current, start);
            current = start;
            break;
        }
        case PathCommandType::CubicBezier: {
            Gdiplus::PointF p1(cmd.params[0], cmd.params[1]);
            Gdiplus::PointF p2(cmd.params[2], cmd.params[3]);
            Gdiplus::PointF p3(cmd.params[4], cmd.params[5]);
            gp.AddBezier(current, p1, p2, p3);
            current = p3;
            break;
        }
        case PathCommandType::SmoothCubicBezier: {
            Gdiplus::PointF p1(cmd.params[0], cmd.params[1]); // đã tính sẵn trong parse
            Gdiplus::PointF p2(cmd.params[2], cmd.params[3]);
            Gdiplus::PointF p3(cmd.params[4], cmd.params[5]);
            gp.AddBezier(current, p1, p2, p3);
            current = p3;
            break;
        }
        }
    }

    const SVGStyle& st = path->getSVGStyle();
    Color fillColor = st.getGdiFillColor();
    Stroke* strokeObj = st.getStroke();

    if (fillColor.GetA() > 0) {
        SolidBrush brush(fillColor);
        g.FillPath(&brush, &gp);
    }
    if (strokeObj) {
        Color strokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;
        if (strokeW <= 0.0f) strokeW = 1.0f;
        Pen pen(strokeColor, strokeW);
        g.DrawPath(&pen, &gp);
    }

    g.SetTransform(&oldMatrix);
}

