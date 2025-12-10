#pragma once
#include "SVGGroup.h"
#include <string>

class SVGFactoryPattern;
class SVGElement;
class Stroke;
class SVGStyle;

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
}

class SVGParser {
private:
	tinyxml2::XMLElement* currentNode = nullptr;

public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element);

	SVGGroup* readXML(const std::string& filename, SVGFactoryPattern& factory);
	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory);

	void parseStroke(Stroke& stroke, const std::string& styleStr);
	void parseStyle(SVGStyle& style, const std::string& styleStr);
};