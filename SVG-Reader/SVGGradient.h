//#pragma once
//#include <vector>
//#include <string>
//#include <windows.h>
//#include <gdiplus.h>
//#include "tinyxml2.h"
//#include "CustomColor.h"
//
//namespace Gdiplus {
//    class LinearGradientBrush;
//}
//
//enum class GradientType {
//    Linear,
//    Radial
//};
//
//struct GradientStop {
//    float offset; // 0.0f -> 1.0f
//    CustomColor color;
//    float opacity;
//};
//
//class SVGGradient {
//private:
//    std::string id;
//    GradientType type;
//
//    // Toạ độ cho Linear (x1, y1, x2, y2) hoặc Radial (cx, cy, r, fx, fy)
//    // Lưu dưới dạng phần trăm (0.0 - 1.0) hoặc tuyệt đối
//    float x1, y1, x2, y2;
//
//    std::vector<GradientStop> stops;
//    std::string units; // "userSpaceOnUse" hoặc "objectBoundingBox" (default)
//
//public:
//    SVGGradient(const std::string& id, GradientType type);
//    ~SVGGradient();
//
//    void parse(tinyxml2::XMLElement* node);
//    void addStop(float offset, CustomColor color, float opacity);
//
//    std::string getId() const { return id; }
//    GradientType getType() const { return type; }
//
//    // Hàm tạo Brush GDI+ dựa trên khung bao (bounding box) của hình vẽ
//    Gdiplus::Brush* createBrush(const Gdiplus::RectF& bounds);
//};