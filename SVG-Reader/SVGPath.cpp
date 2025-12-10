#include "SVGPath.h"
#include "SVGRenderer.h"
#include "tinyxml2.h"
#include <cctype>
#include <sstream>
#include <vector>

SVGPath::SVGPath() : SVGElement() {
    this->setTagName("path");
}

SVGElement* SVGPath::clone() const {
    return new SVGPath(*this);
}

// Helper: Bỏ qua khoảng trắng và dấu phẩy
static void skipSpacesAndCommas(const std::string& s, size_t& pos) {
    while (pos < s.size()) {
        char ch = s[pos];
        if (std::isspace((unsigned char)ch) || ch == ',')
            ++pos;
        else
            break;
    }
}

// Helper: Đọc 1 số float
static bool parseFloatToken(const std::string& s, size_t& pos, float& out) {
    skipSpacesAndCommas(s, pos);
    if (pos >= s.size()) return false;

    size_t start = pos;
    // Dấu +/-
    if (s[pos] == '+' || s[pos] == '-') ++pos;

    bool hasDigit = false;
    while (pos < s.size() && (std::isdigit((unsigned char)s[pos]) || s[pos] == '.')) {
        hasDigit = true;
        ++pos;
    }
    // Số mũ e/E
    if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
        ++pos;
        if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) ++pos;
        while (pos < s.size() && std::isdigit((unsigned char)s[pos])) {
            hasDigit = true;
            ++pos;
        }
    }

    if (!hasDigit) return false;

    try {
        out = std::stof(s.substr(start, pos - start));
        return true;
    }
    catch (...) { return false; }
}

void SVGPath::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);

    const char* dAttr = node->Attribute("d");
    if (!dAttr) return;

    std::string d = dAttr;
    size_t pos = 0;
    char cmd = 0;

    float currentX = 0.0f, currentY = 0.0f;
    float startX = 0.0f, startY = 0.0f;
    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;

    while (pos < d.size()) {
        skipSpacesAndCommas(d, pos);
        if (pos >= d.size()) break;

        char c = d[pos];
        // Nếu là ký tự chữ cái -> là lệnh mới
        if (std::isalpha((unsigned char)c)) {
            cmd = c;
            ++pos;
        }
        // Nếu không phải chữ cái -> dùng lại lệnh cũ (implicit command)

        bool isRel = (cmd >= 'a' && cmd <= 'z');
        char absCmd = (char)std::toupper((unsigned char)cmd);

        switch (absCmd) {
        case 'M': {
            float x, y;
            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
            if (isRel) { x += currentX; y += currentY; }

            PathCommand pc;
            pc.type = PathCommandType::MoveTo;
            pc.params = { x, y };
            commands.push_back(pc);

            currentX = x; currentY = y;
            startX = x; startY = y;
            // Sau M/m, nếu còn số thì hiểu là L/l
            cmd = (isRel) ? 'l' : 'L';
            break;
        }
        case 'L': {
            float x, y;
            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
            if (isRel) { x += currentX; y += currentY; }

            PathCommand pc;
            pc.type = PathCommandType::LineTo;
            pc.params = { x, y };
            commands.push_back(pc);
            currentX = x; currentY = y;
            break;
        }
        case 'H': {
            float x;
            if (!parseFloatToken(d, pos, x)) break;
            if (isRel) x += currentX;

            PathCommand pc;
            pc.type = PathCommandType::HLineTo;
            pc.params = { x };
            commands.push_back(pc);
            currentX = x;
            break;
        }
        case 'V': {
            float y;
            if (!parseFloatToken(d, pos, y)) break;
            if (isRel) y += currentY;

            PathCommand pc;
            pc.type = PathCommandType::VLineTo;
            pc.params = { y };
            commands.push_back(pc);
            currentY = y;
            break;
        }
        case 'C': {
            float x1, y1, x2, y2, x, y;
            if (!parseFloatToken(d, pos, x1) || !parseFloatToken(d, pos, y1) ||
                !parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;

            if (isRel) {
                x1 += currentX; y1 += currentY;
                x2 += currentX; y2 += currentY;
                x += currentX; y += currentY;
            }

            PathCommand pc;
            pc.type = PathCommandType::CubicBezier;
            pc.params = { x1, y1, x2, y2, x, y };
            commands.push_back(pc);

            lastCtrlX = x2; lastCtrlY = y2;
            currentX = x; currentY = y;
            break;
        }
        case 'S': {
            float x2, y2, x, y;
            if (!parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;

            float x1 = currentX, y1 = currentY;
            // Tính điểm điều khiển đối xứng
            if (!commands.empty()) {
                PathCommandType prevType = commands.back().type;
                if (prevType == PathCommandType::CubicBezier || prevType == PathCommandType::SmoothCubicBezier) {
                    x1 = 2.0f * currentX - lastCtrlX;
                    y1 = 2.0f * currentY - lastCtrlY;
                }
            }

            if (isRel) {
                x2 += currentX; y2 += currentY;
                x += currentX; y += currentY;
            }

            PathCommand pc;
            pc.type = PathCommandType::SmoothCubicBezier;
            // Lưu luôn 3 điểm để render cho dễ
            pc.params = { x1, y1, x2, y2, x, y };
            commands.push_back(pc);

            lastCtrlX = x2; lastCtrlY = y2;
            currentX = x; currentY = y;
            break;
        }
        case 'Z': {
            PathCommand pc;
            pc.type = PathCommandType::ClosePath;
            commands.push_back(pc);
            currentX = startX; currentY = startY;
            break;
        }
        default:
            ++pos; // Bỏ qua lệnh chưa hỗ trợ (Q, A...)
            break;
        }
    }
}

void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderPath(g, this);
}