#include "SVGText.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGText::SVGText()
    : SVGElement("text", "", SVGStyle()),
    font_size(12.0f),
    start(CustomPoint(0.0f, 0.0f)),
    content(L"")
{
}
SVGText::SVGText(float font_size, const CustomPoint& start, const std::wstring& content)
    : SVGElement("text", "", SVGStyle()),
    font_size(font_size),
    start(start),
    content(content)
{
}
SVGText::SVGText(const SVGText& other) {
    this->tag_name = other.tag_name;
    this->id = other.id;
    this->style = other.style;

    this->font_size = other.font_size;
    this->start = other.start;
    this->content = other.content;
}
SVGText& SVGText::operator=(const SVGText& other) {
    if (this != &other) {
        this->tag_name = other.tag_name;
        this->id = other.id;
        this->style = other.style;

        this->font_size = other.font_size;
        this->start = other.start;
        this->content = other.content;
    }
    return *this;
}
//SVGText::~SVGText() {}
SVGElement* SVGText::clone() const {
    return new SVGText(*this);
}

float SVGText::getFontSize() const {
    return font_size;
}
void SVGText::setFontSize(float font_size) {
    this->font_size = font_size;
}
CustomPoint SVGText::getStart() const {
    return start;
}
void SVGText::setStart(const CustomPoint& start) {
    this->start = start;
}
std::wstring SVGText::getContent() const {
    return content;
}
void SVGText::setContent(const std::wstring& content) {
    this->content = content;
}

void SVGText::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);

    float x = node->FloatAttribute("x");
    float y = node->FloatAttribute("y");
    this->setStart(CustomPoint(x, y));

    float fontSize = node->FloatAttribute("font-size");

    if (fontSize <= 0.0f) {
        fontSize = 12.0f; 
    }
    this->setFontSize(fontSize);

    const char* content_char = node->GetText();

    if (content_char) {
        std::string content_str(content_char);
        this->setContent(std::wstring(content_str.begin(), content_str.end()));
    }
    else {
        this->setContent(L"");
    }
}
void SVGText::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderText(g, this);
}
// void SVGText::transform(Matrix* m) {
