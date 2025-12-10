#include "SVGText.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"
#include <windows.h> 
#include <algorithm>

SVGText::SVGText()
    : SVGElement("text", "", SVGStyle()), font_size(12.0f), start(0, 0), content(L"") {}

SVGText::SVGText(float font_size, const CustomPoint& start, const std::wstring& content)
    : SVGElement("text", "", SVGStyle()), font_size(font_size), start(start), content(content) {}

SVGText::SVGText(const SVGText& other) : SVGElement(other) {
    this->font_size = other.font_size;
    this->start = other.start;
    this->content = other.content;
}

SVGText& SVGText::operator=(const SVGText& other) {
    if (this != &other) {
        SVGElement::operator=(other);
        this->font_size = other.font_size;
        this->start = other.start;
        this->content = other.content;
    }
    return *this;
}

SVGElement* SVGText::clone() const {
    return new SVGText(*this);
}

// Getters/Setters
float SVGText::getFontSize() const { return font_size; }
void SVGText::setFontSize(float fs) { font_size = fs; }
CustomPoint SVGText::getStart() const { return start; }
void SVGText::setStart(const CustomPoint& s) { start = s; }
std::wstring SVGText::getContent() const { return content; }
void SVGText::setContent(const std::wstring& c) { content = c; }

void SVGText::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);

    float x = node->FloatAttribute("x");
    float y = node->FloatAttribute("y");
    this->setStart(CustomPoint(x, y));

    float fontSize = node->FloatAttribute("font-size");
    if (fontSize <= 0.0f) fontSize = 12.0f;
    this->setFontSize(fontSize);

    const char* content_char = node->GetText();
    if (!content_char && node->FirstChild()) {
        auto child = node->FirstChild();
        if (child->ToText()) {
            content_char = child->Value();
        }
    }

    if (content_char) {
        // 1. Convert sang WideString
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, content_char, -1, NULL, 0);
        std::wstring ws(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, content_char, -1, &ws[0], size_needed);
        if (!ws.empty() && ws.back() == L'\0') ws.pop_back();

        // 2. QUAN TRỌNG: Thay thế xuống dòng/tab thành dấu cách
        // Để "sleep all \n day" thành "sleep all day"
        std::replace(ws.begin(), ws.end(), L'\n', L' ');
        std::replace(ws.begin(), ws.end(), L'\r', L' ');
        std::replace(ws.begin(), ws.end(), L'\t', L' ');

        // (Tùy chọn) Xóa khoảng trắng thừa kép nếu cần, nhưng replace đơn giản là đủ
        this->setContent(ws);
    }
    else {
        this->setContent(L"");
    }
}

void SVGText::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderText(g, this);
}