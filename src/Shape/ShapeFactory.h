#pragma once

#include <stdexcept>
#include "Shape.h"
#include "../StrokeSettings.h"

struct ShapeFactory
{
    static Shape CreateShape(const StrokeSettings &settings, wxPoint2DDouble origin)
    {
        wxPoint2DDouble origin2D{origin};
        switch (settings.currentShape)
        {
        case ShapeType::Path:
            return Path{settings.color, settings.width, {origin2D}};
        case ShapeType::Rect:
            return Rect{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.outlineWidth, settings.gradientStops, settings.isGradient};
        case ShapeType::Circle:
            return Circle{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.outlineWidth, settings.gradientStops, settings.isGradient};
        case ShapeType::ITriangle:
            return ITriangle{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.outlineWidth, settings.gradientStops, settings.isGradient};
        case ShapeType::RTriangle:
            return RTriangle{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.outlineWidth, settings.gradientStops, settings.isGradient};
        case ShapeType::Diamond:
            return Diamond{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.outlineWidth, settings.gradientStops, settings.isGradient};
        case ShapeType::Text:
            return Text{{origin2D.m_x, origin2D.m_y, 300, 100}, "", settings.textColor, settings.fontSize, settings.font};
        default:
            throw std::runtime_error("Unknown shape type");
        }
    }
};
