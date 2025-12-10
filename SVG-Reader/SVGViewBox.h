#pragma once
#include <string>
#include <vector>

struct SVGViewBox {
    float x;      // min-x
    float y;      // min-y
    float width;
    float height;

    SVGViewBox();
    SVGViewBox(float x, float y, float w, float h);

    // Hàm parse chuỗi "0 0 100 100" thành object
    static SVGViewBox parse(const std::string& viewBoxStr);

    // Kiểm tra xem viewBox có hợp lệ không
    bool isValid() const;
};