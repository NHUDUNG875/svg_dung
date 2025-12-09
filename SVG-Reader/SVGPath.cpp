#include "SVGPath.h"
#include "SVGRenderer.h"
#include "tinyxml2.h"
#include <cctype>
#include <sstream>

SVGPath::SVGPath() : SVGElement() {
    this->setTagName("path");
}

SVGElement* SVGPath::clone() const {
    return new SVGPath(*this);
}

// --- hàm phụ: đọc 1 số float từ chuỗi, tự skip space/comma ---
static bool readFloat(const string& s, size_t& pos, float& out) {
    while (pos < s.size() && (isspace((unsigned char)s[pos]) || s[pos] == ',')) {
        ++pos;
    }
    if (pos >= s.size()) return false;

    size_t start = pos;
 
    if (s[pos] == '+' || s[pos] == '-') ++pos;
    while (pos < s.size() && (isdigit((unsigned char)s[pos]) || s[pos] == '.')) {
        ++pos;
    }
    if (start == pos) return false;

    try {
        out = stof(s.substr(start, pos - start));
    }
    catch (...) {
        return false;
    }
    return true;
}

static void skipSpaces(const std::string& s, size_t& pos) {
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) {
        ++pos;
    }
}

static bool parseFloat(const std::string& s, size_t& pos, float& out) {
    skipSpaces(s, pos);
    if (pos >= s.size()) return false;

    size_t start = pos;
    if (s[pos] == '+' || s[pos] == '-') {
        ++pos;
    }

    bool hasDigit = false;
    while (pos < s.size() && (std::isdigit(static_cast<unsigned char>(s[pos])) || s[pos] == '.')) {
        hasDigit = true;
        ++pos;
    }

    if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
        ++pos;
        if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
            ++pos;
        }
        while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) {
            hasDigit = true;
            ++pos;
        }
    }

    if (!hasDigit) return false;

    try {
        out = std::stof(s.substr(start, pos - start));
        return true;
    }
    catch (...) {
        return false;
    }
}


static std::vector<float> parseNumberList(const std::string& s, size_t& pos, int expectedCount) {
    std::vector<float> nums;
    nums.reserve(expectedCount);

    for (int i = 0; i < expectedCount; ++i) {
        skipSpaces(s, pos);

        if (pos < s.size() && s[pos] == ',') {
            ++pos;
            skipSpaces(s, pos);
        }

        float value;
        if (!parseFloat(s, pos, value)) {
            break;
        }
        nums.push_back(value);
    }

    return nums;
}

// đọc thuộc tính d 
void SVGPath::parse(tinyxml2::XMLElement* node) {
    const char* dAttr = node->Attribute("d");
    if (!dAttr) return;

    std::string d = dAttr;
    size_t pos = 0;
    char cmd = 0;
    float currentX = 0.0f, currentY = 0.0f;
    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;
    while (pos < d.size()) {
        // bỏ khoảng trắng
        while (pos < d.size() && isspace((unsigned char)d[pos])) ++pos;
        if (pos >= d.size()) break;

        char c = d[pos];
        if (isalpha((unsigned char)c)) {
            cmd = c;          // GIỮ nguyên hoa/thường
            ++pos;
        }
        if (cmd == 0) { ++pos; continue; }

        switch (cmd) {
            // ---------- M / m ----------
        case 'M':
        case 'm': {
            bool isRel = (cmd == 'm');
            float x, y;
            if (!readFloat(d, pos, x) || !readFloat(d, pos, y)) break;

            if (isRel) { x += currentX; y += currentY; }

            PathCommand pc;
            pc.type = PathCommandType::MoveTo;
            pc.params = { x, y };
            commands.push_back(pc);
            currentX = x; currentY = y;

            // các cặp tiếp theo (nếu có) coi như L / l
            while (true) {
                float x2, y2;
                size_t savePos = pos;
                if (!readFloat(d, pos, x2) || !readFloat(d, pos, y2)) {
                    pos = savePos;
                    break;
                }
                if (isRel) { x2 += currentX; y2 += currentY; }

                PathCommand lc;
                lc.type = PathCommandType::LineTo;
                lc.params = { x2, y2 };
                commands.push_back(lc);
                currentX = x2; currentY = y2;
            }
            break;
        }

                // ---------- L / l ----------
        case 'L':
        case 'l': {
            bool isRel = (cmd == 'l');
            while (true) {
                float x, y;
                size_t savePos = pos;
                if (!readFloat(d, pos, x) || !readFloat(d, pos, y)) {
                    pos = savePos;
                    break;
                }
                if (isRel) { x += currentX; y += currentY; }

                PathCommand pc;
                pc.type = PathCommandType::LineTo;
                pc.params = { x, y };
                commands.push_back(pc);
                currentX = x; currentY = y;
            }
            break;
        }

                // ---------- H / h ----------
        case 'H':
        case 'h': {
            bool isRel = (cmd == 'h');
            while (true) {
                float x;
                size_t savePos = pos;
                if (!readFloat(d, pos, x)) {
                    pos = savePos;
                    break;
                }
                if (isRel) x += currentX;

                PathCommand pc;
                pc.type = PathCommandType::HLineTo;
                pc.params = { x };
                commands.push_back(pc);
                currentX = x;
            }
            break;
        }

                // ---------- V / v ----------
        case 'V':
        case 'v': {
            bool isRel = (cmd == 'v');
            while (true) {
                float y;
                size_t savePos = pos;
                if (!readFloat(d, pos, y)) {
                    pos = savePos;
                    break;
                }
                if (isRel) y += currentY;

                PathCommand pc;
                pc.type = PathCommandType::VLineTo;
                pc.params = { y };
                commands.push_back(pc);
                currentY = y;
            }
            break;
        }

                // ---------- C / c ----------
// ---------- C / c ----------
// ---------- C / c ----------
        case 'C':
        case 'c': {
            bool isRel = (cmd == 'c');
            while (true) {
                float x1, y1, x2, y2, x, y;
                size_t savePos = pos;
                if (!readFloat(d, pos, x1) || !readFloat(d, pos, y1) ||
                    !readFloat(d, pos, x2) || !readFloat(d, pos, y2) ||
                    !readFloat(d, pos, x) || !readFloat(d, pos, y)) {
                    pos = savePos;
                    break;
                }

                if (isRel) {
                    x1 += currentX; y1 += currentY;
                    x2 += currentX; y2 += currentY;
                    x += currentX; y += currentY;
                }

                PathCommand pc;
                pc.type = PathCommandType::CubicBezier;
                pc.params = { x1, y1, x2, y2, x, y };
                commands.push_back(pc);

                // Cập nhật: điểm điều khiển cuối là control 2
                lastCtrlX = x2;
                lastCtrlY = y2;
                currentX = x;
                currentY = y;
            }
            break;
        }

                // ---------- S / s ----------
        case 'S':
        case 's': {
            bool isRel = (cmd == 's');
            while (true) {
                float x2, y2, x, y;
                size_t savePos = pos;
                if (!readFloat(d, pos, x2) || !readFloat(d, pos, y2) ||
                    !readFloat(d, pos, x) || !readFloat(d, pos, y)) {
                    pos = savePos;
                    break;
                }

                // Nếu lệnh trước đó KHÔNG phải C/c/S/s thì
                // điểm điều khiển 1 trùng với current point
                float x1, y1;
                if (commands.empty()) {
                    x1 = currentX;
                    y1 = currentY;
                }
                else {
                    PathCommandType prevType = commands.back().type;
                    if (prevType == PathCommandType::CubicBezier ||
                        prevType == PathCommandType::SmoothCubicBezier) {
                        // phản chiếu control cuối qua current point
                        x1 = 2.0f * currentX - lastCtrlX;
                        y1 = 2.0f * currentY - lastCtrlY;
                    }
                    else {
                        x1 = currentX;
                        y1 = currentY;
                    }
                }

                if (isRel) {
                    x2 += currentX; y2 += currentY;
                    x += currentX; y += currentY;
                }

                PathCommand pc;
                pc.type = PathCommandType::SmoothCubicBezier;
                pc.params = { x1, y1, x2, y2, x, y };
                commands.push_back(pc);

                lastCtrlX = x2;
                lastCtrlY = y2;
                currentX = x;
                currentY = y;
            }
            break;
        }

                // ---------- Z / z ----------
        case 'Z':
        case 'z': {
            PathCommand pc;
            pc.type = PathCommandType::ClosePath;
            commands.push_back(pc);
            ++pos;
            break;
        }

                // ---------- lệnh khác: bỏ qua ----------
        default:
            ++pos;
            break;
        }
    }
}
void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderPath(g, this);
}