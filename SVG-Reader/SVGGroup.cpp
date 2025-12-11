#include "SVGGroup.h"
#include "SVGRenderer.h" 
#include "SVGFactoryPattern.h"
#include <algorithm>
#include <iostream> // <--- QUAN TRỌNG: Sửa lỗi C2676 (ostream)

// --- Constructor & Destructor ---
SVGGroup::SVGGroup() : SVGElement(), ElementArray{}, parent(nullptr) {}

SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other), parent(other.parent) {
    this->width = other.width;
    this->height = other.height;
    this->viewBox = other.viewBox;

    for (auto element : other.ElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) this->ElementArray.push_back(newElement);
    }
}

SVGGroup::~SVGGroup() {
    for (auto element : ElementArray) delete element;
    ElementArray.clear();
}

// --- Swap & Operator= ---
void swap(SVGGroup& first, SVGGroup& second) noexcept {
    using std::swap;
    swap(first.ElementArray, second.ElementArray);
    swap(first.parent, second.parent);
    swap(first.width, second.width);
    swap(first.height, second.height);
    swap(first.viewBox, second.viewBox);
}

SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
    SVGElement::operator=(other);
    swap(*this, other);
    return *this;
}

// --- Getters & Setters ---
void SVGGroup::setParent(SVGGroup* parent) { this->parent = parent; }

void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {
    for (auto elem : ElementArray) delete elem;
    ElementArray.clear();
    for (auto element : SVGElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) this->ElementArray.push_back(newElement);
    }
}

void SVGGroup::addElement(SVGElement* ele) { this->ElementArray.push_back(ele); }

SVGGroup* SVGGroup::getParent() { return this->parent; }

const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const { return this->ElementArray; }

SVGElement* SVGGroup::clone() const { return new SVGGroup(*this); }

// --- Parsing ---
void SVGGroup::parse(tinyxml2::XMLElement* node) {
    SVGElement::parse(node);

    if (this->getTagName() == "svg") {
        const char* viewBoxStr = node->Attribute("viewBox");
        if (viewBoxStr) {
            this->viewBox = SVGViewBox::parse(viewBoxStr);
        }

        float attrW = node->FloatAttribute("width");
        float attrH = node->FloatAttribute("height");

        // Fallback size to HD (1280x720) if not specified, prevents clipping
        if (attrW > 0) this->setWidth(attrW);
        else if (this->viewBox.isValid()) this->setWidth(this->viewBox.width);
        else this->setWidth(1280.0f);

        if (attrH > 0) this->setHeight(attrH);
        else if (this->viewBox.isValid()) this->setHeight(this->viewBox.height);
        else this->setHeight(720.0f);
    }
}

// --- Render ---
void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderGroup(g, this);
}

void SVGGroup::printDebugAttributes(std::ostream& os) const {
    os << "Group(tag=" << getTagName() << ", id=" << getId() << ", children=" << ElementArray.size() << ")\n";
}