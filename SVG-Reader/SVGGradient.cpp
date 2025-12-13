//#include "SVGGradient.h"
//#include <algorithm>
//#include <cmath>
//
//
//using namespace Gdiplus;
//
//SVGGradient::SVGGradient(const std::string& id, GradientType type)
//    : id(id), type(type), x1(0), y1(0), x2(1), y2(0), units("objectBoundingBox")
//{
//    // Mặc định Linear Gradient chạy từ trái sang phải (0,0) -> (1,0)
//}
//
//SVGGradient::~SVGGradient() {}
//
//void SVGGradient::addStop(float offset, CustomColor color, float opacity) {
//    stops.push_back({ offset, color, opacity });
//}
//
//// Helper: Chuyển chuỗi phần trăm hoặc số thập phân sang float 0.0-1.0
//static float parseUnit(const char* str, float defaultVal) {
//    if (!str) return defaultVal;
//    std::string s = str;
//    if (s.back() == '%') {
//        try { return std::stof(s.substr(0, s.size() - 1)) / 100.0f; }
//        catch (...) { return defaultVal; }
//    }
//    try { return std::stof(s); }
//    catch (...) { return defaultVal; }
//}
//
//void SVGGradient::parse(tinyxml2::XMLElement* node) {
//    // 1. Đọc thuộc tính toạ độ
//    if (type == GradientType::Linear) {
//        this->x1 = parseUnit(node->Attribute("x1"), 0.0f);
//        this->y1 = parseUnit(node->Attribute("y1"), 0.0f);
//        this->x2 = parseUnit(node->Attribute("x2"), 1.0f); // Mặc định phải là 100%
//        this->y2 = parseUnit(node->Attribute("y2"), 0.0f);
//    }
//
//    // Đơn vị toạ độ (quan trọng để scale brush)
//    const char* unitAttr = node->Attribute("gradientUnits");
//    if (unitAttr) this->units = unitAttr;
//
//    // 2. Đọc các thẻ <stop> con
//    for (tinyxml2::XMLElement* child = node->FirstChildElement("stop"); child; child = child->NextSiblingElement("stop")) {
//        float offset = parseUnit(child->Attribute("offset"), 0.0f);
//
//        // Đọc màu stop-color
//        CustomColor color;
//        const char* stopColor = child->Attribute("stop-color");
//        if (stopColor) color = CustomColor::fromStringToCustomColor(stopColor);
//        else {
//            // Check style="stop-color:..."
//            const char* style = child->Attribute("style");
//            if (style) {
//                std::string s = style;
//                if (s.find("stop-color") != std::string::npos) {
//                    // Cần tách chuỗi style đơn giản (đây là bản rút gọn)
//                    size_t pos = s.find("stop-color:");
//                    if (pos != std::string::npos) {
//                        size_t end = s.find(';', pos);
//                        std::string val = s.substr(pos + 11, end - (pos + 11));
//                        color = CustomColor::fromStringToCustomColor(val);
//                    }
//                }
//            }
//        }
//
//        // Đọc stop-opacity
//        float opacity = 1.0f;
//        const char* stopOpacity = child->Attribute("stop-opacity");
//        if (stopOpacity) opacity = std::stof(stopOpacity);
//
//        this->addStop(offset, color, opacity);
//    }
//}
//
//Gdiplus::Brush* SVGGradient::createBrush(const Gdiplus::RectF& bounds) {
//    if (stops.empty()) return new SolidBrush(Color::Black);
//
//    // Tính toán điểm thực tế dựa trên BoundingBox của hình
//    PointF start, end;
//
//    if (units == "objectBoundingBox") {
//        start.X = bounds.X + x1 * bounds.Width;
//        start.Y = bounds.Y + y1 * bounds.Height;
//        end.X = bounds.X + x2 * bounds.Width;
//        end.Y = bounds.Y + y2 * bounds.Height;
//    }
//    else {
//        // userSpaceOnUse: Toạ độ tuyệt đối (đơn giản hoá cho bài tập)
//        start.X = x1; start.Y = y1;
//        end.X = x2;   end.Y = y2;
//    }
//
//    // Xử lý trường hợp start == end (tránh lỗi GDI+)
//    if (abs(start.X - end.X) < 0.001f && abs(start.Y - end.Y) < 0.001f) {
//        end.X += 0.1f;
//    }
//
//    // Tạo LinearGradientBrush
//    // Lưu ý: LinearGradientBrush mặc định chỉ nhận 2 màu.
//    // Để dùng nhiều stop (multicolor), ta cần dùng SetInterpolationColors.
//
//    // Lấy màu đầu và cuối tạm thời để khởi tạo
//    Color c1 = stops.front().color;
//    Color c2 = stops.back().color;
//
//    LinearGradientBrush* brush = new LinearGradientBrush(start, end, c1, c2);
//
//    // Thiết lập Multicolor (Preset Colors)
//    int count = (int)stops.size();
//    if (count > 0) {
//        Color* colors = new Color[count];
//        float* positions = new float[count];
//
//        for (int i = 0; i < count; i++) {
//            BYTE a = (BYTE)(stops[i].opacity * 255.0f);
//            Color base = stops[i].color;
//            colors[i] = Color(a, base.GetR(), base.GetG(), base.GetB());
//            positions[i] = stops[i].offset;
//        }
//
//        brush->SetInterpolationColors(colors, positions, count);
//
//        delete[] colors;
//        delete[] positions;
//    }
//
//    return brush;
//}