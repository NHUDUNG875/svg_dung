#include <windows.h>
#include <gdiplus.h>
#include "SVGRenderer.h"
#include "Stroke.h"
#include "SVGRectangle.h"
#include "SVGCircle.h"
#include "SVGEllipse.h"
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGText.h"
#include "SVGSquare.h"
#include "SVGElement.h"
#include "SVGGroup.h"
#include "SVGTransform.h"
#include "SVGPath.h"

using Gdiplus::PointF;
using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::Color;
using Gdiplus::Matrix;

SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}

void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; rect->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    Gdiplus::Point p = rect->getTopLeftCorner();
    float w = rect->getWidth(); float h = rect->getHeight();
    SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
    g.FillRectangle(&brush, (float)p.X, (float)p.Y, w, h);

    Stroke* strokeObj = rect->getSVGStyle().getStroke();
    if (strokeObj) {
        Pen pen(strokeObj->getGdiColor(), strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f);
        g.DrawRectangle(&pen, (float)p.X, (float)p.Y, w, h);
    }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; circle->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    PointF c = circle->getCenter(); float r = circle->getRadius();
    SolidBrush brush(circle->getSVGStyle().getGdiFillColor());
    g.FillEllipse(&brush, c.X - r, c.Y - r, 2 * r, 2 * r);
    Stroke* s = circle->getSVGStyle().getStroke();
    if (s) { Pen p(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawEllipse(&p, c.X - r, c.Y - r, 2 * r, 2 * r); }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* el) {
    if (!el) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; el->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    PointF c = el->getCenter(); float rx = el->getRadiusX(), ry = el->getRadiusY();
    SolidBrush brush(el->getSVGStyle().getGdiFillColor());
    g.FillEllipse(&brush, c.X - rx, c.Y - ry, 2 * rx, 2 * ry);
    Stroke* s = el->getSVGStyle().getStroke();
    if (s) { Pen p(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawEllipse(&p, c.X - rx, c.Y - ry, 2 * rx, 2 * ry); }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* l) {
    if (!l) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; l->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    Stroke* s = l->getSVGStyle().getStroke();
    if (s) { Pen p(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawLine(&p, (PointF)l->getStartPoint(), (PointF)l->getEndPoint()); }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* p) {
    if (!p) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; p->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    std::vector<PointF> pts; for (auto cp : p->getPoints()) pts.emplace_back(cp.x, cp.y);
    if (!pts.empty()) {
        SolidBrush b(p->getSVGStyle().getGdiFillColor()); g.FillPolygon(&b, pts.data(), (INT)pts.size());
        Stroke* s = p->getSVGStyle().getStroke(); if (s) { Pen pen(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawPolygon(&pen, pts.data(), (INT)pts.size()); }
    }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* p) {
    if (!p) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; p->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    std::vector<PointF> pts; for (auto cp : p->getPoints()) pts.emplace_back(cp.x, cp.y);
    if (pts.size() > 1) {
        if (p->getSVGStyle().getGdiFillColor().GetA() > 0) { SolidBrush b(p->getSVGStyle().getGdiFillColor()); g.FillPolygon(&b, pts.data(), (INT)pts.size()); }
        Stroke* s = p->getSVGStyle().getStroke(); if (s) { Pen pen(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawLines(&pen, pts.data(), (INT)pts.size()); }
    }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* sq) {
    if (!sq) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; sq->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);
    PointF p = sq->getTopLeftCorner(); float s_len = sq->getWidth();
    SolidBrush b(sq->getSVGStyle().getGdiFillColor()); g.FillRectangle(&b, p.X, p.Y, s_len, s_len);
    Stroke* s = sq->getSVGStyle().getStroke(); if (s) { Pen pen(s->getGdiColor(), s->strokeWidth > 0 ? s->strokeWidth : 1.0f); g.DrawRectangle(&pen, p.X, p.Y, s_len, s_len); }
    g.SetTransform(&oldMatrix);
}

// Thay thế toàn bộ hàm renderText cũ bằng hàm này:
// Tìm và thay thế hàm renderText cũ bằng hàm này:
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);

    // 1. Áp dụng Transform
    Matrix localMatrix;
    text->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    PointF pos = text->getStart();
    std::wstring content = text->getContent();

    Color fillColor = text->getSVGStyle().getGdiFillColor();
    Stroke* strokeObj = text->getSVGStyle().getStroke();

    float fontSize = text->getFontSize();
    if (fontSize <= 0.0f) fontSize = 12.0f;

    Gdiplus::FontFamily fontFamily(L"Times New Roman");
    int fontStyle = Gdiplus::FontStyleRegular;

    // Kiểm tra in nghiêng/đậm (nếu cần phát triển thêm sau này)
    // Hiện tại mặc định Regular cho giống mẫu

    // 2. Xử lý căn lề (Text Anchor)
    Gdiplus::StringFormat format;
    // SVG mặc định là 'start' (Near), nhưng trong ví dụ này có 'middle'
    // Để đơn giản, ta hardcode kiểm tra hoặc mặc định Center nếu thấy x > 0
    // (Trong bài này ta set Center cho giống hình mẫu)
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear); // Căn theo dòng trên cùng (Baseline xử lý sau)

    // Tính Baseline offset
    float emHeight = (float)fontFamily.GetEmHeight(fontStyle);
    float cellAscent = (float)fontFamily.GetCellAscent(fontStyle);
    float ascentPixels = fontSize * (cellAscent / emHeight);

    // 3. Tạo Path chữ
    Gdiplus::GraphicsPath path;
    // Vẽ tại đúng toạ độ (x, y - ascent)
    path.AddString(
        content.c_str(),
        -1,
        &fontFamily,
        fontStyle,
        fontSize,
        PointF(pos.X, pos.Y - ascentPixels),
        &format
    );

    // 4. Tô màu (Fill)
    if (fillColor.GetA() > 0) {
        SolidBrush brush(fillColor);
        g.FillPath(&brush, &path);
    }

    // 5. Vẽ viền (Stroke) - Quan trọng để hiện chữ đậm/viền màu
    if (strokeObj) {
        Color strokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
        Pen pen(strokeColor, strokeW);
        g.DrawPath(&pen, &path);
    }

    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);
}

void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix groupMatrix; rootGroup->getTransform().applyToMatrix(groupMatrix);
    g.MultiplyTransform(&groupMatrix);

    for (SVGElement* element : rootGroup->getSVGElementArray()) {
        if (element) element->render(*this, g);
    }
    g.SetTransform(&oldMatrix);
}

void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
    if (!path) return;
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; path->getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

    Gdiplus::GraphicsPath gp;
    PointF current(0.0f, 0.0f);

    // Mặc định SVG dùng luật Winding, nhưng GDI+ mặc định là Alternate.
    // Nếu hình vẫn bị thủng lỗ sai, hãy thử bật dòng dưới:
    // gp.SetFillMode(Gdiplus::FillModeWinding);

    for (const auto& cmd : path->getCommands()) {
        switch (cmd.type) {
        case PathCommandType::MoveTo: {
            gp.StartFigure(); // QUAN TRỌNG: Ngắt nét để không nối với điểm cũ
            current = PointF(cmd.params[0], cmd.params[1]);
            break;
        }
        case PathCommandType::LineTo: {
            PointF p(cmd.params[0], cmd.params[1]);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::HLineTo: {
            PointF p(cmd.params[0], current.Y);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::VLineTo: {
            PointF p(current.X, cmd.params[0]);
            gp.AddLine(current, p);
            current = p;
            break;
        }
        case PathCommandType::ClosePath: {
            gp.CloseFigure(); // QUAN TRỌNG: Đóng hình chuẩn
            Gdiplus::PointF lastPoint;
            gp.GetLastPoint(&lastPoint);
            current = lastPoint;
            break;
        }
        case PathCommandType::CubicBezier: {
            PointF p1(cmd.params[0], cmd.params[1]);
            PointF p2(cmd.params[2], cmd.params[3]);
            PointF p3(cmd.params[4], cmd.params[5]);
            gp.AddBezier(current, p1, p2, p3);
            current = p3;
            break;
        }
        case PathCommandType::SmoothCubicBezier: {
            PointF p1(cmd.params[0], cmd.params[1]);
            PointF p2(cmd.params[2], cmd.params[3]);
            PointF p3(cmd.params[4], cmd.params[5]);
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
        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
        Pen pen(strokeColor, strokeW);
        g.DrawPath(&pen, &gp);
    }
    g.SetTransform(&oldMatrix);
}
//#include <windows.h>
//#include <gdiplus.h>
//#include "SVGRenderer.h"
//#include "Stroke.h"
//#include "SVGRectangle.h"
//#include "SVGCircle.h"
//#include "SVGEllipse.h"
//#include "SVGLine.h"
//#include "SVGPolygon.h"
//#include "SVGPolyline.h"
//#include "SVGText.h"
//#include "SVGSquare.h"
//#include "SVGElement.h"
//#include "SVGGroup.h"
//#include "SVGTransform.h"
//#include "SVGPath.h"
//
//using Gdiplus::PointF;
//using Gdiplus::SolidBrush;
//using Gdiplus::Pen;
//using Gdiplus::Color;
//using Gdiplus::Matrix;
//
//SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}
//
//void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
//    if (!rect) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    rect->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    Gdiplus::Point p = rect->getTopLeftCorner();
//    PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
//    float w = rect->getWidth();
//    float h = rect->getHeight();
//
//    SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
//    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
//
//    Stroke* strokeObj = rect->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) strokeW = 1.0f;
//
//        Pen pen(strokeColor, strokeW);
//        g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
//    }
//
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
//    if (!circle) return;
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    circle->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    PointF center = circle->getCenter();
//    float radius = circle->getRadius();
//
//    SolidBrush brush(circle->getSVGStyle().getGdiFillColor());
//
//    float diameter = radius * 2;
//    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);
//
//    Stroke* strokeObj = circle->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) {
//            strokeW = 1.0f;
//        }
//        Pen pen(strokeColor, strokeW);
//        g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
//    if (!ellipse) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    ellipse->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    PointF center = ellipse->getCenter();
//    float rx = ellipse->getRadiusX();
//    float ry = ellipse->getRadiusY();
//
//    SolidBrush brush(ellipse->getSVGStyle().getGdiFillColor());
//    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//
//    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) strokeW = 1.0f;
//
//        Pen pen(strokeColor, strokeW);
//        g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//    }
//
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
//    if (!line) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    line->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    PointF start = line->getStartPoint();
//    PointF end = line->getEndPoint();
//
//    Stroke* strokeObj = line->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) strokeW = 1.0f;
//
//        Pen pen(strokeColor, strokeW);
//        g.DrawLine(&pen, start, end);
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
//    if (!polygon) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    polygon->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
//    std::vector<PointF> gdiPoints;
//    gdiPoints.reserve(customPoints.size());
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.empty()) {
//        g.SetTransform(&oldMatrix);
//        return;
//    }
//
//    SolidBrush brush(polygon->getSVGStyle().getGdiFillColor());
//    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//
//    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) strokeW = 1.0f;
//
//        Pen pen(strokeColor, strokeW);
//        g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
//    if (!polyline) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    polyline->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    std::vector<PointF> gdiPoints;
//    const auto& customPoints = polyline->getPoints();
//
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//
//    if (gdiPoints.size() < 2) {
//        g.SetTransform(&oldMatrix);
//        return;
//    }
//
//    Color fillColor = polyline->getSVGStyle().getGdiFillColor();
//
//    if (fillColor.GetA() > 0) {
//        SolidBrush brush(fillColor);
//        g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//
//    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//
//        if (strokeW <= 0.0f) {
//            strokeW = 1.0f;
//        }
//
//        Pen pen(finalStrokeColor, strokeW);
//        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
//    if (!text) return;
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    text->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    PointF pos = text->getStart();
//    std::wstring content = text->getContent();
//    Color color = text->getSVGStyle().getGdiFillColor();
//    float fontSize = text->getFontSize();
//
//    if (fontSize <= 0.0f) fontSize = 12.0f;
//
//    Gdiplus::FontFamily fontFamily(L"Times New Roman");
//    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//    SolidBrush brush(color);
//
//    // DÙNG pos trực tiếp (không trừ fontSize để tránh text bị chạy quá lên trên)
//    PointF new_pos(pos.X, pos.Y);
//    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
//
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
//    if (!square) return;
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    square->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    PointF topLeft = square->getTopLeftCorner();
//    float side = square->getHeight();
//    SolidBrush brush(square->getSVGStyle().getGdiFillColor());
//
//    Stroke* strokeObj = square->getSVGStyle().getStroke();
//    Color strokeColor = strokeObj ? strokeObj->getGdiColor() : Color(255, 0, 0, 0);
//    float strokeW = (strokeObj && strokeObj->strokeWidth > 0.0f) ? strokeObj->strokeWidth : 1.0f;
//
//    Pen pen(strokeColor, strokeW);
//    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
//    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
//
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
//    if (!rootGroup) return;
//    rootGroup->render(*this, g);
//}
//
//void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
//    if (!rootGroup) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix groupMatrix;
//    rootGroup->getTransform().applyToMatrix(groupMatrix);
//
//    g.MultiplyTransform(&groupMatrix);
//
//    const auto& children = rootGroup->getSVGElementArray();
//    for (SVGElement* element : children) {
//        if (element) {
//            element->render(*this, g);
//        }
//    }
//
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
//    if (!path) return;
//
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Matrix localMatrix;
//    path->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    Gdiplus::GraphicsPath gp;
//    PointF current(0.0f, 0.0f);
//    PointF start(0.0f, 0.0f);
//
//    for (const auto& cmd : path->getCommands()) {
//        switch (cmd.type) {
//        case PathCommandType::MoveTo: {
//            float x = cmd.params[0], y = cmd.params[1];
//            current = PointF(x, y);
//            start = current;
//            break;
//        }
//        case PathCommandType::LineTo: {
//            float x = cmd.params[0], y = cmd.params[1];
//            PointF p(x, y);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::HLineTo: {
//            float x = cmd.params[0];
//            PointF p(x, current.Y);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::VLineTo: {
//            float y = cmd.params[0];
//            PointF p(current.X, y);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::ClosePath: {
//            gp.AddLine(current, start);
//            current = start;
//            break;
//        }
//        case PathCommandType::CubicBezier: {
//            PointF p1(cmd.params[0], cmd.params[1]);
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            gp.AddBezier(current, p1, p2, p3);
//            current = p3;
//            break;
//        }
//        case PathCommandType::SmoothCubicBezier: {
//            PointF p1(cmd.params[0], cmd.params[1]); // đã tính sẵn trong parse
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            gp.AddBezier(current, p1, p2, p3);
//            current = p3;
//            break;
//        }
//        }
//    }
//
//    const SVGStyle& st = path->getSVGStyle();
//    Color fillColor = st.getGdiFillColor();
//    Stroke* strokeObj = st.getStroke();
//
//    if (fillColor.GetA() > 0) {
//        SolidBrush brush(fillColor);
//        g.FillPath(&brush, &gp);
//    }
//    if (strokeObj) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) strokeW = 1.0f;
//        Pen pen(strokeColor, strokeW);
//        g.DrawPath(&pen, &gp);
//    }
//
//    g.SetTransform(&oldMatrix);
//}