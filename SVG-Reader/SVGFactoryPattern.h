#pragma once
#include <string>
#include <unordered_map>
#include "SVGElement.h"

class SVGFactoryPattern {
private:
    std::unordered_map<std::string, int> ElementID;
public:
    SVGFactoryPattern();
    SVGElement* getElement(std::string tagname);
    const std::unordered_map<std::string, int>& getElementID() const;
};

