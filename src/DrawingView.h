#pragma once

#include <wx/docview.h>

#include "DrawingDocument.h"
#include "Canvas/CanvasObject.h"
#include "Canvas/ShapeCreator.h"
#include "Canvas/SelectionBox.h"

class DrawingView : public wxView
{
public:
    bool OnCreate(wxDocument *doc, long flags) override;

    void OnDraw(wxDC *dc) override;

    void OnMouseDown(wxPoint);
    void OnMouseDrag(wxPoint);
    void OnMouseDragEnd();

    void OnClear();
    bool OnClose(bool deleteWindow = true) override;

    void PredefinedRotate(double angle);

    // Setting the Frame title
    void OnChangeFilename() override;

    DrawingDocument *GetDocument() const;
    CanvasObject GetCanvasObject();
    ShapeCreator &GetShapeCreator();

    wxDECLARE_DYNAMIC_CLASS(DrawingView);

private:
    void AddPointToCurrentLine(wxPoint);

private:
    wxPoint lastDragStart{};
    ShapeCreator shapeCreator{};
    std::optional<SelectionBox> selectionBox{};
};