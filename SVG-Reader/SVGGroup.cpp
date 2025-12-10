#include "SVGGroup.h"
#include "SVGRenderer.h" // Bắt buộc include để render
#include "SVGFactoryPattern.h"
#include <algorithm>

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

// --- Getters & Setters (Đây là các hàm bị lỗi LNK2019) ---
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

        if (attrW > 0) this->setWidth(attrW);
        else if (this->viewBox.isValid()) this->setWidth(this->viewBox.width);
        else this->setWidth(800.0f);

        if (attrH > 0) this->setHeight(attrH);
        else if (this->viewBox.isValid()) this->setHeight(this->viewBox.height);
        else this->setHeight(600.0f);
    }
}

// --- Render ---
void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderGroup(g, this);
}

void SVGGroup::printDebugAttributes(std::ostream& os) const {
    os << "Group(tag=" << getTagName() << ", id=" << getId() << ", children=" << ElementArray.size() << ")\n";
}
// ... (Giữ nguyên các hàm render, printDebug...)
//#include "SVGGroup.h"
//#include "SVGRenderer.h" // Include này rất quan trọng
//#include "SVGFactoryPattern.h"
//#include <algorithm>
//
//SVGGroup::SVGGroup() : SVGElement(), ElementArray{}, parent(nullptr) {}
//SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other), parent(other.parent) {
//    for (auto element : other.ElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) this->ElementArray.push_back(newElement);
//    }
//}
//void swap(SVGGroup& first, SVGGroup& second) noexcept {
//    using std::swap;
//    swap(first.ElementArray, second.ElementArray);
//    swap(first.parent, second.parent);
//    swap(first.width, second.width);
//    swap(first.height, second.height);
//    swap(first.viewBox, second.viewBox);
//}
//SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
//    SVGElement::operator=(other);
//    swap(*this, other);
//    return *this;
//}
//SVGGroup::~SVGGroup() {
//    for (auto element : ElementArray) delete element;
//    ElementArray.clear();
//}
//void SVGGroup::setParent(SVGGroup* parent) { this->parent = parent; }
//void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {
//    for (auto elem : ElementArray) delete elem;
//    ElementArray.clear();
//    for (auto element : SVGElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) this->ElementArray.push_back(newElement);
//    }
//}
//void SVGGroup::addElement(SVGElement* ele) { this->ElementArray.push_back(ele); }
//SVGGroup* SVGGroup::getParent() { return this->parent; }
//const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const { return this->ElementArray; }
//SVGElement* SVGGroup::clone() const { return new SVGGroup(*this); }
//
//void SVGGroup::parse(tinyxml2::XMLElement* node) {
//    SVGElement::parse(node);
//    if (this->getTagName() == "svg") {
//        float w = node->FloatAttribute("width"); if (w <= 0.0f) w = 300.0f; this->setWidth(w);
//        float h = node->FloatAttribute("height"); if (h <= 0.0f) h = 150.0f; this->setHeight(h);
//        const char* vb = node->Attribute("viewBox");
//        if (vb) this->setViewBox(std::string(vb));
//        else this->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
//    }
//}
//void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g) const { r.renderGroup(g, this); }
//void SVGGroup::printDebugAttributes(std::ostream& os) const {
//    os << "Group(tag=" << getTagName() << ", id=" << getId() << ", children=" << ElementArray.size() << ")\n";
//}
//#include "SVGGroup.h"
//#include "SVGRenderer.h" // <--- QUAN TRỌNG: Thêm dòng này để sửa lỗi C2027
//#include "SVGFactoryPattern.h"
//#include <algorithm>
//#include <string>
//
//SVGGroup::SVGGroup() :
//    SVGElement(),
//    ElementArray{},
//    parent(nullptr) {
//}
//
//SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()), parent(other.parent) {
//    for (auto element : other.ElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) {
//            this->ElementArray.push_back(newElement);
//        }
//    }
//}
//
//void swap(SVGGroup& first, SVGGroup& second) noexcept {
//    using std::swap;
//    swap(first.ElementArray, second.ElementArray);
//    swap(first.parent, second.parent);
//}
//
//SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
//    swap(*this, other);
//    return *this;
//}
//
//SVGGroup::~SVGGroup() {
//    for (auto element : ElementArray)
//        delete element;
//    ElementArray.clear();
//}
//
//void SVGGroup::setParent(SVGGroup* parent) {
//    this->parent = parent;
//}
//
//void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {
//
//    for (auto elem : ElementArray)
//        delete elem;
//    ElementArray.clear();
//
//
//    for (auto element : SVGElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) {
//            this->ElementArray.push_back(newElement);
//        }
//    }
//}
//
//void SVGGroup::addElement(SVGElement* ele) {
//    this->ElementArray.push_back(ele);
//}
//
//SVGGroup* SVGGroup::getParent() {
//    return this->parent;
//}
//
//const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const {
//    return this->ElementArray;
//}
//
//SVGElement* SVGGroup::clone() const {
//    return new SVGGroup(*this);
//}
//
//void SVGGroup::parse(tinyxml2::XMLElement* node) {
//    SVGElement::parse(node);
//    // parser.parseGroup(this, node);
//    if (std::string(node->Name()) == "svg") {
//
//        float w = node->FloatAttribute("width");
//        if (w <= 0.0f) { w = 300.0f; }
//        this->setWidth(w);
//
//
//        float h = node->FloatAttribute("height");
//        if (h <= 0.0f) { h = 150.0f; }
//        this->setHeight(h);
//
//
//        const char* viewBoxStr = node->Attribute("viewBox");
//        if (viewBoxStr) {
//            this->setViewBox(std::string(viewBoxStr));
//        }
//        else {
//            this->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
//        }
//    }
//}
//
//void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
//    r.renderGroup(g, this);
//}
//
//void SVGGroup::printDebugAttributes(std::ostream& os) const {
//    os << "Group(tag=" << getTagName()
//        << ", id=" << getId()
//        << ", children=" << ElementArray.size()
//        << ")\n";
//}