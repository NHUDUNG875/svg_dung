#pragma once
#include "SVGGroup.h"

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
	class XMLNode;
	class XMLText;
	class XMLComment;
}
class SVGParser {
private:
	tinyxml2::XMLElement* currentNode = nullptr;
	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup);

public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element);
	SVGGroup* readXML(const std::string& filename, SVGFactoryPattern& factory);

	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory);
	void parseStroke(Stroke& stroke, const std::string& styleStr);
	void parseStyle(SVGStyle& style, const std::string& styleStr);

    
	// void parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node);
	// void parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node);
	// void parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node);
	// void parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node);
	// void parseLine(SVGLine* line, tinyxml2::XMLElement* Node);
	// void parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node);
	// void parseText(SVGText* text, tinyxml2::XMLElement* Node);
	// void parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node);

};



