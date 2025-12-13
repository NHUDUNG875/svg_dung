#include "SVGElement.h"
#include "tinyxml2.h"
#include "SVGParser.h"
#include <iostream>

SVGElement::SVGElement() {
    this->tag_name = "";
    this->id = "";
    this->style = SVGStyle();
}
SVGElement::SVGElement(const std::string& tag, const std::string& identifier, const SVGStyle& svg_style) {
    this->tag_name = tag;
    this->id = identifier;
    this->style = svg_style;
}

// it is unnescessary write copy constructor, copy assignment and destructor 
// because they are delete and default => force us: override at derived classes

// SVGElement::SVGElement(const SVGElement&);
// SVGElement& SVGElement::operator=(const SVGElement&); 
// SVGElement::~SVGElement();

std::string SVGElement::getTagName() const {
    return tag_name;
}
void SVGElement::setTagName(const std::string& tag) {
    this->tag_name = tag;
}
std::string SVGElement::getId() const {
    return id;
}
void SVGElement::setId(const std::string& identifier) {
    this->id = identifier;
}

SVGStyle& SVGElement::getSVGStyle() {
    return style;
}
const SVGStyle& SVGElement::getSVGStyle() const {
    return style;
}

void SVGElement::setSVGStyle(const SVGStyle& svg_style) {
    this->style = svg_style;
}

// polymorphism and separation of concerns
void SVGElement::parse(tinyxml2::XMLElement* node) {
    // assume that: g_parser is global variable
    extern SVGParser g_parser;
    g_parser.parseAttributes(node, this);
}

// void SVGElement::render(SVGRenderer&, Gdiplus::Graphics&) const {}     
// virtual void SVGElement::transform(Matrix* m) = 0; // pure virtual function
// Trong SVGElement.cpp (thêm hàm này)