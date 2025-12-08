#include "SVGFactoryPattern.h"
#include "SVGElement.h"
#include <stdexcept>
#include <algorithm>
#include "SVGEllipse.h"
#include "SVGCircle.h"
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGRectangle.h"
#include "SVGSquare.h"
#include "SVGText.h"
#include "SVGGroup.h"

SVGFactoryPattern::SVGFactoryPattern() {
    ElementID["circle"] = 1;
    ElementID["ellipse"] = 2;
    ElementID["line"] = 3;
    ElementID["polygon"] = 4;
    ElementID["polyline"] = 5;
    ElementID["rect"] = 6;
    ElementID["square"] = 7;
    ElementID["text"] = 8;
    ElementID["g"] = 9;
}


SVGElement* SVGFactoryPattern::getElement(std::string tagname) {
    auto it = ElementID.find(tagname);
    if (it == ElementID.end()) {
        throw std::out_of_range("Cannot find the type of object for tag: " + tagname);
    }

    int numid = it->second;

    switch (numid) {
    case 1:
        return new SVGCircle();
    case 2:
        return new SVGEllipse();
    case 3:
        return new SVGLine();
    case 4:
        return new SVGPolygon();
    case 5:
        return new SVGPolyline();
    case 6:
        return new SVGRectangle();
    case 7:
        return new SVGSquare();
    case 8:
        return new SVGText();
    case 9:
        return new SVGGroup();
    default:
        throw std::runtime_error("Found tag - " + tagname + " but mapped ID is invalid");
    }
}

const std::unordered_map<std::string, int>& SVGFactoryPattern::getElementID() const {
    return this->ElementID;
}
