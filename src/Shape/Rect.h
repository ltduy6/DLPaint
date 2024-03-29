#pragma once

#include <wx/graphics.h>

struct Rect
{
    wxRect2DDouble rect;
    wxColour outlineColor;
    wxColour fillColor;
    int width;
    wxGraphicsGradientStops gradientStops;
    bool isGradient;
};