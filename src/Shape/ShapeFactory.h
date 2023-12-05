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
            return Rect{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.width};
        case ShapeType::Circle:
            return Circle{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.width};
        case ShapeType::ITriangle:
            return ITriangle{{origin2D.m_x, origin2D.m_y, 0, 0}, origin2D, origin2D, settings.outlineColor, settings.fillColor, settings.width};
        case ShapeType::RTriangle:
            return RTriangle{{origin2D.m_x, origin2D.m_y, 0, 0}, origin2D, origin2D, settings.outlineColor, settings.fillColor, settings.width};
        case ShapeType::Diamond:
            return Diamond{{origin2D.m_x, origin2D.m_y, 0, 0}, settings.outlineColor, settings.fillColor, settings.width};
        case ShapeType::Text:
            return Text{{origin2D.m_x, origin2D.m_y, 300, 100}, "Hello", settings.textColor, settings.width};
        default:
            throw std::runtime_error("Unknown shape type");
        }
    }
};
