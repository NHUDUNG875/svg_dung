// SVGPath.h
#pragma once
#include <windows.h> 
#include <gdiplus.h>
#include "SVGElement.h"
#include <vector>
using namespace Gdiplus;
using namespace std;

enum class PathCommandType {
    MoveTo,
    LineTo,
    HLineTo,
    VLineTo,
    ClosePath,
    CubicBezier,
    SmoothCubicBezier
};

struct PathCommand {
    PathCommandType type;
    vector<float> params;
};

class SVGPath : public SVGElement {
private:
    vector<PathCommand> commands;

public:
    SVGPath();
    SVGElement* clone() const override;

    void parse(tinyxml2::XMLElement* node) override;
    void render(SVGRenderer& r, Graphics& g) const override;

    const vector<PathCommand>& getCommands() const { return commands; }
};