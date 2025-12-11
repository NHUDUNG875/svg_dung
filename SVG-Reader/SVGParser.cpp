//#include "SVGParser.h"
//#include "SVGFactoryPattern.h" 
//#include "SVGElement.h"
//#include "SVGGroup.h"
//#include "tinyxml2.h"
//#include "Stroke.h"     
//#include "SVGStyle.h"   
//#include "SVGTransform.h"
//#include <sstream>
//#include <algorithm>
//#include <iostream>
//#include <vector>
//
//static void parseColorString(const std::string& val, CustomColor& outColor, float& outOpacity, bool& hasAlpha) {
//    std::string s = val;
//    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
//
//    if (s.rfind("rgba(", 0) == 0) {
//        size_t start = s.find('(');
//        size_t end = s.find(')');
//        if (start != std::string::npos && end != std::string::npos) {
//            std::string content = s.substr(start + 1, end - start - 1);
//            std::stringstream ss(content);
//            std::string segment;
//            int r = 0, g = 0, b = 0;
//            float a = 1.0f;
//            try {
//                if (std::getline(ss, segment, ',')) r = std::stoi(segment);
//                if (std::getline(ss, segment, ',')) g = std::stoi(segment);
//                if (std::getline(ss, segment, ',')) b = std::stoi(segment);
//                if (std::getline(ss, segment, ',')) a = std::stof(segment);
//                outColor = CustomColor(r, g, b);
//                outOpacity = a;
//                hasAlpha = true;
//                return;
//            }
//            catch (...) {}
//        }
//    }
//    outColor = CustomColor::fromStringToCustomColor(val);
//    outOpacity = 1.0f;
//    hasAlpha = false;
//}
//
//void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
//    SVGStyle& currentStyle = element->getSVGStyle();
//
//    const tinyxml2::XMLAttribute* strokeAttr = xmlNode->FindAttribute("stroke");
//    const tinyxml2::XMLAttribute* widthAttr = xmlNode->FindAttribute("stroke-width");
//    const tinyxml2::XMLAttribute* opacityAttr = xmlNode->FindAttribute("stroke-opacity");
//
//    if ((strokeAttr || widthAttr || opacityAttr) && currentStyle.getStroke() == nullptr) {
//        Stroke tempStroke;
//        currentStyle.setStroke(&tempStroke);
//    }
//
//    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
//    while (attr) {
//        std::string name = attr->Name();
//        std::string value = attr->Value();
//
//        if (name == "id") element->setId(value);
//        else if (name == "style") this->parseStyle(currentStyle, value);
//        else if (name == "fill") {
//            if (value == "none") currentStyle.setFillOpacity(0.0f);
//            else {
//                CustomColor c; float alpha = 1.0f; bool hasAlpha = false;
//                parseColorString(value, c, alpha, hasAlpha);
//                currentStyle.setFillColor(c);
//                if (hasAlpha) currentStyle.setFillOpacity(alpha);
//                else if (currentStyle.getFillOpacity() <= 0.0f) currentStyle.setFillOpacity(1.0f);
//            }
//        }
//        else if (name == "fill-opacity") { try { currentStyle.setFillOpacity(std::stof(value)); } catch (...) {} }
//        else if (name == "stroke" || name == "stroke-width" || name == "stroke-opacity") {
//            Stroke* s = currentStyle.getStroke();
//            if (!s) { Stroke temp; currentStyle.setStroke(&temp); s = currentStyle.getStroke(); }
//            if (s) {
//                if (name == "stroke") s->strokeColor = CustomColor::fromStringToCustomColor(value);
//                else if (name == "stroke-width") try { s->strokeWidth = std::stof(value); }
//                catch (...) {}
//                else if (name == "stroke-opacity") try { s->strokeOpacity = std::stof(value); }
//                catch (...) {}
//            }
//        }
//        attr = attr->Next();
//    }
//
//    const char* transformStr = xmlNode->Attribute("transform");
//    if (transformStr) {
//        SVGTransform t = SVGTransform::parse(transformStr);
//        element->setTransform(t);
//    }
//    element->setSVGStyle(currentStyle);
//}
//
//SVGGroup* SVGParser::readXML(const std::string& filename, SVGFactoryPattern& factory) {
//    tinyxml2::XMLDocument doc;
//    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) return nullptr;
//    tinyxml2::XMLElement* rootXML = doc.RootElement();
//    if (!rootXML) return nullptr;
//
//    SVGGroup* rootGroup = new SVGGroup();
//    rootGroup->setParent(nullptr);
//    rootGroup->setTagName(rootXML->Name());
//
//    parseAttributes(rootXML, rootGroup);
//    rootGroup->parse(rootXML);
//    parseNode(rootXML, rootGroup, factory);
//    return rootGroup;
//}
//
//void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory) {
//    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
//        std::string tagName = child->Name();
//        SVGElement* element = factory.getElement(tagName);
//        if (!element) continue;
//
//        parseAttributes(child, element);
//        element->parse(child);
//        parentGroup->addElement(element);
//
//        if (auto group = dynamic_cast<SVGGroup*>(element)) {
//            group->setParent(parentGroup);
//            parseNode(child, group, factory);
//        }
//    }
//}
//
//void SVGParser::parseStyle(SVGStyle& style, const std::string& styleStr) {
//    std::stringstream ss(styleStr);
//    std::string item;
//    while (std::getline(ss, item, ';')) {
//        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
//        if (item.empty()) continue;
//        size_t pos = item.find(':'); if (pos == std::string::npos) continue;
//        std::string key = item.substr(0, pos); std::string value = item.substr(pos + 1);
//
//        if (key == "fill") {
//            if (value == "none") style.setFillOpacity(0.0f);
//            else {
//                style.setFillColor(CustomColor::fromStringToCustomColor(value));
//                if (style.getFillOpacity() <= 0.0f) style.setFillOpacity(1.0f);
//            }
//        }
//        else if (key == "fill-opacity") {
//            try { style.setFillOpacity(std::stof(value)); }
//            catch (...) {}
//        }
//        else if (key.find("stroke") != std::string::npos) {
//            Stroke* s = style.getStroke();
//            if (!s) { Stroke t; style.setStroke(&t); s = style.getStroke(); }
//            if (s) parseStroke(*s, item);
//        }
//    }
//}
//
//void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
//    std::string s = styleStr;
//    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
//    size_t pos = s.find(':'); if (pos == std::string::npos) return;
//    std::string key = s.substr(0, pos); std::string value = s.substr(pos + 1);
//
//    if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
//    else if (key == "stroke-width") try { stroke.strokeWidth = std::stof(value); }
//    catch (...) { stroke.strokeWidth = 1.0f; }
//    else if (key == "stroke-opacity") try { stroke.strokeOpacity = std::stof(value); }
//    catch (...) { stroke.strokeOpacity = 1.0f; }
//}
#include "SVGParser.h"
#include "SVGFactoryPattern.h" 
#include "SVGElement.h"
#include "SVGGroup.h"
#include "tinyxml2.h"
#include "Stroke.h"     
#include "SVGStyle.h"   
#include "SVGTransform.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>

static void parseColorString(const std::string& val, CustomColor& outColor, float& outOpacity, bool& hasAlpha) {
    std::string s = val;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    if (s.rfind("rgba(", 0) == 0) {
        size_t start = s.find('(');
        size_t end = s.find(')');
        if (start != std::string::npos && end != std::string::npos) {
            std::string content = s.substr(start + 1, end - start - 1);
            std::stringstream ss(content);
            std::string segment;
            int r = 0, g = 0, b = 0;
            float a = 1.0f;
            try {
                if (std::getline(ss, segment, ',')) r = std::stoi(segment);
                if (std::getline(ss, segment, ',')) g = std::stoi(segment);
                if (std::getline(ss, segment, ',')) b = std::stoi(segment);
                if (std::getline(ss, segment, ',')) a = std::stof(segment);
                outColor = CustomColor(r, g, b);
                outOpacity = a;
                hasAlpha = true;
                return;
            } catch (...) {}
        }
    }
    outColor = CustomColor::fromStringToCustomColor(val);
    outOpacity = 1.0f;
    hasAlpha = false;
}

void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
    SVGStyle& currentStyle = element->getSVGStyle();

    const tinyxml2::XMLAttribute* strokeAttr = xmlNode->FindAttribute("stroke");
    const tinyxml2::XMLAttribute* widthAttr = xmlNode->FindAttribute("stroke-width");
    const tinyxml2::XMLAttribute* opacityAttr = xmlNode->FindAttribute("stroke-opacity");

    if ((strokeAttr || widthAttr || opacityAttr) && currentStyle.getStroke() == nullptr) {
        Stroke tempStroke;
        currentStyle.setStroke(&tempStroke);
    }

    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
    while (attr) {
        std::string name = attr->Name();
        std::string value = attr->Value();

        if (name == "id") element->setId(value);
        else if (name == "style") this->parseStyle(currentStyle, value);
        else if (name == "fill") {
            if (value == "none") currentStyle.setFillOpacity(0.0f);
            else {
                CustomColor c; float alpha = 1.0f; bool hasAlpha = false;
                parseColorString(value, c, alpha, hasAlpha);
                currentStyle.setFillColor(c);
                if (hasAlpha) currentStyle.setFillOpacity(alpha);
                else if (currentStyle.getFillOpacity() <= 0.0f) currentStyle.setFillOpacity(1.0f);
            }
        }
        else if (name == "fill-opacity") { try { currentStyle.setFillOpacity(std::stof(value)); } catch (...) {} }
        else if (name == "stroke" || name == "stroke-width" || name == "stroke-opacity") {
            Stroke* s = currentStyle.getStroke();
            if (!s) { Stroke temp; currentStyle.setStroke(&temp); s = currentStyle.getStroke(); }
            if (s) {
                if (name == "stroke") s->strokeColor = CustomColor::fromStringToCustomColor(value);
                else if (name == "stroke-width") try { s->strokeWidth = std::stof(value); } catch (...) {}
                else if (name == "stroke-opacity") try { s->strokeOpacity = std::stof(value); } catch (...) {}
            }
        }
        else if (name == "font-style") {
            if (value == "italic") {
                // Dùng bitwise OR để kết hợp style (2 là giá trị của FontStyleItalic trong GDI+)
                currentStyle.setFontStyle(currentStyle.getFontStyle() | 2);
            }
        }
        else if (name == "font-weight") {
            if (value == "bold" || value == "bolder" || value == "700") {
                // Dùng bitwise OR để kết hợp style (1 là giá trị của FontStyleBold)
                currentStyle.setFontStyle(currentStyle.getFontStyle() | 1);
            }
        }
        std::string fillRule;
        attr = attr->Next();
    }

    const char* transformStr = xmlNode->Attribute("transform");
    if (transformStr) {
        SVGTransform t = SVGTransform::parse(transformStr);
        element->setTransform(t);
    }
    element->setSVGStyle(currentStyle);
}

SVGGroup* SVGParser::readXML(const std::string& filename, SVGFactoryPattern& factory) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) return nullptr;
    tinyxml2::XMLElement* rootXML = doc.RootElement();
    if (!rootXML) return nullptr;

    SVGGroup* rootGroup = new SVGGroup();
    rootGroup->setParent(nullptr);
    rootGroup->setTagName(rootXML->Name());

    parseAttributes(rootXML, rootGroup);
    rootGroup->parse(rootXML);
    parseNode(rootXML, rootGroup, factory);
    return rootGroup;
}

// Tìm đến hàm parseNode và sửa lại như sau:
void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory) {
    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string tagName = child->Name();
        SVGElement* element = factory.getElement(tagName);
        if (!element) continue;

        // --- SỬA LỖI KẾ THỪA (INHERITANCE) ---
        // Nếu có group cha, copy style của cha xuống con trước (để lấy stroke, fill mặc định)
        if (parentGroup) {
            element->setSVGStyle(parentGroup->getSVGStyle());
        }
        // --------------------------------------

        parseAttributes(child, element);
        element->parse(child);
        parentGroup->addElement(element);

        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group, factory);
        }
    }
}

void SVGParser::parseStyle(SVGStyle& style, const std::string& styleStr) {
    std::stringstream ss(styleStr);
    std::string item;
    while (std::getline(ss, item, ';')) {
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;
        size_t pos = item.find(':'); if (pos == std::string::npos) continue;
        std::string key = item.substr(0, pos); std::string value = item.substr(pos + 1);

        if (key == "fill") {
            if (value == "none") style.setFillOpacity(0.0f);
            else {
                style.setFillColor(CustomColor::fromStringToCustomColor(value));
                if (style.getFillOpacity() <= 0.0f) style.setFillOpacity(1.0f);
            }
        } else if (key == "fill-opacity") {
            try { style.setFillOpacity(std::stof(value)); } catch (...) {}
        } else if (key.find("stroke") != std::string::npos) {
            Stroke* s = style.getStroke();
            if (!s) { Stroke t; style.setStroke(&t); s = style.getStroke(); }
            if (s) parseStroke(*s, item);
        }
    }
}

void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
    std::string s = styleStr;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t pos = s.find(':'); if (pos == std::string::npos) return;
    std::string key = s.substr(0, pos); std::string value = s.substr(pos + 1);

    if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
    else if (key == "stroke-width") try { stroke.strokeWidth = std::stof(value); } catch (...) { stroke.strokeWidth = 1.0f; }
    else if (key == "stroke-opacity") try { stroke.strokeOpacity = std::stof(value); } catch (...) { stroke.strokeOpacity = 1.0f; }
}