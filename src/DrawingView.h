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
    void SetExporting(bool isExporting);

    void OnMouseDown(wxPoint);
    void OnMouseDrag(wxPoint);
    void OnMouseDragEnd();
    void OnKeyDown(wxKeyEvent &event);

    void OnClear();
    bool OnClose(bool deleteWindow = true) override;

    void PredefinedRotate(double angle);
    void Refresh();
    void ResetModified();
    void SetCenter(wxPoint);
    void SetScaleObjects(double scaleFactor, wxPoint2DDouble center);
    void SetVirtualSize(wxSize size);
    void SetCanvasBound(wxRect bound);
    void SetZoomFactor(double zoomFactor, wxPoint2DDouble center);

    bool GetIsModified() const;
    bool GetIsSelected() const;

    // Setting the Frame title
    void OnChangeFilename() override;

    DrawingDocument *GetDocument() const;
    CanvasObject GetCanvasObject();
    ShapeCreator &GetShapeCreator();
    SelectionBox &GetSelectionBox();

    wxDECLARE_DYNAMIC_CLASS(DrawingView);

private:
    void AddPointToCurrentLine(wxPoint);
    wxChar GetCharFromKeycode(int keycode);

private:
    wxPoint lastDragStart{};
    wxPoint screenCenter{};
    wxSize m_virtualSize{};
    wxRect m_canvasBound{};
    double m_zoomFactor{1};
    ShapeCreator shapeCreator{};
    std::optional<SelectionBox> selectionBox{};
    bool isModified{false};
    bool isExporting{false};
};