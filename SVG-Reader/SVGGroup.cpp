#include "SVGGroup.h"
#include "SVGFactoryPattern.h"

#include <algorithm>

SVGGroup::SVGGroup() :
    SVGElement(),
    ElementArray{},
    parent(nullptr) {
}

SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()), parent(other.parent) {
    for (auto element : other.ElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}

void swap(SVGGroup& first, SVGGroup& second) noexcept {
    using std::swap;
    swap(first.ElementArray, second.ElementArray);
    swap(first.parent, second.parent);
}

SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
    swap(*this, other);
    return *this;
}

SVGGroup::~SVGGroup() {
    for (auto element : ElementArray)
        delete element;
    ElementArray.clear();
}

void SVGGroup::setParent(SVGGroup* parent) {
    this->parent = parent;
}

void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {

    for (auto elem : ElementArray)
        delete elem;
    ElementArray.clear();


    for (auto element : SVGElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}

void SVGGroup::addElement(SVGElement* ele) {
    this->ElementArray.push_back(ele);
}

SVGGroup* SVGGroup::getParent() {
    return this->parent;
}

const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const {
    return this->ElementArray;
}

SVGElement* SVGGroup::clone() const {
    return new SVGGroup(*this);
}

void SVGGroup::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);
    // parser.parseGroup(this, node);
    if (std::string(node->Name()) == "svg") {

        float w = node->FloatAttribute("width");
        if (w <= 0.0f) { w = 300.0f; }
        this->setWidth(w);


        float h = node->FloatAttribute("height");
        if (h <= 0.0f) { h = 150.0f; }
        this->setHeight(h);


        const char* viewBoxStr = node->Attribute("viewBox");
        if (viewBoxStr) {
            this->setViewBox(std::string(viewBoxStr));
        }
        else {
            this->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
        }
    }
}

void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderGroup(g, this);
}

void SVGGroup::printDebugAttributes(std::ostream& os) const {
    os << "Group(tag=" << getTagName()
        << ", id=" << getId()
        << ", children=" << ElementArray.size()
        << ")\n";
}