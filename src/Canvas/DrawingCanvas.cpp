#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "DrawingCanvas.h"
#include "../Command/AddCommand.h"
#include "../Command/SelectionCommand.h"
#include "../Command/ShapeCommand.h"
#include "../Command/DeleteCommand.h"
#include "../Menu/ZoomToolMenu.h"
#include "../MyApp.h"
#include <iostream>

DrawingCanvas::DrawingCanvas(wxWindow *parent, DrawingView *view, HistoryPanel &historyPanel, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size), view(view), m_historyPanel(historyPanel)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);
    view->Bind(wxEVT_CHAR_HOOK, &DrawingCanvas::OnKeyDown, this);
    this->Bind(wxEVT_MOUSEWHEEL, &DrawingCanvas::OnScroll, this);

    m_initialSize = parent->GetSize();
}

void DrawingCanvas::ShowExportDialog()
{
    if (view)
    {
        wxFileDialog exportFileDialog(this, _("Export drawing"), "", "",
                                      "Bitmap Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (exportFileDialog.ShowModal() == wxID_CANCEL)
            return;

        wxBitmap bitmap(m_initialSize);

        wxMemoryDC memDC;

        memDC.SetUserScale(this->GetContentScaleFactor(), this->GetContentScaleFactor());
        memDC.SelectObject(bitmap);

        view->SetExporting(true);
        view->OnDraw(&memDC);
        view->SetExporting(false);

        bitmap.SaveFile(exportFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
        bitmap.SaveFile(exportFileDialog.GetPath(), wxBITMAP_TYPE_JPEG);
        bitmap.SaveFile(exportFileDialog.GetPath(), wxBITMAP_TYPE_BMP);
    }
}

void DrawingCanvas::ReFreshCanvas()
{
    view->Refresh();
    Refresh();
}

void DrawingCanvas::RotateCommand()
{
    if (view->GetIsCanRotate() && MyApp::GetStrokeSettings().currentTool == ToolType::Transform)
    {
        view->PredefinedRotate(M_PI / 2);
        Refresh();
        HistoryPane *historyPane = m_historyPanel.get().createHistoryPane("Transform");
        auto command = new SelectionCommand(this, historyPane);
        view->GetDocument()->GetCommandProcessor()->Submit(command);
        m_historyPanel.get().AddHistoryItem(view->GetDocument()->GetCommandProcessor(), historyPane);
    }
}

void DrawingCanvas::SetZoomToolMenu(ZoomToolMenu *zoomToolMenu)
{
    m_zoomToolMenu = zoomToolMenu;
}

void DrawingCanvas::Zoom(double zoomFactor)
{
    view->SetZoomFactor(zoomFactor, wxPoint2DDouble(this->GetParent()->GetClientSize().x / 2, this->GetParent()->GetClientSize().y / 2));
    m_zoomFactor = zoomFactor;
    Refresh();
}

DrawingView *DrawingCanvas::GetView() const
{
    return view;
}

void DrawingCanvas::OnMouseDown(wxMouseEvent &event)
{
    view->OnMouseDown(event.GetPosition());
    isDragging = true;
    Refresh();
}

void DrawingCanvas::OnMouseMove(wxMouseEvent &event)
{
    if (isDragging)
    {
        view->OnMouseDrag(event.GetPosition());
        Refresh();
    }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent &event)
{
    if (MyApp::GetStrokeSettings().currentTool == ToolType::Text)
    {
        isDragging = false;
        if (!view->GetIsModified())
        {
            HistoryPane *historyPane = m_historyPanel.get().createHistoryPane("Text");
            auto command = new AddCommand(this, "Text", historyPane, false);
            view->GetDocument()->GetCommandProcessor()->Submit(command);
            m_historyPanel.get().AddHistoryItem(view->GetDocument()->GetCommandProcessor(), historyPane);
        }
        view->OnMouseDragEnd();
        return;
    }
    if (isDragging)
    {
        view->OnMouseDragEnd();
        isDragging = false;
        if (MyApp::GetStrokeSettings().currentTool == ToolType::Move)
        {
            return;
        }
        if (MyApp::GetStrokeSettings().currentTool == ToolType::Brush || MyApp::GetStrokeSettings().currentTool == ToolType::Shape)
        {
            HistoryPane *historyPane = m_historyPanel.get().createHistoryPane(getShapeCommandName());
            auto command = new AddCommand(this, getShapeCommandName(), historyPane);
            view->GetDocument()->GetCommandProcessor()->Submit(command);
            m_historyPanel.get().AddHistoryItem(view->GetDocument()->GetCommandProcessor(), historyPane);
        }
        if (MyApp::GetStrokeSettings().currentTool == ToolType::Transform && view->GetIsModified())
        {
            HistoryPane *historyPane = m_historyPanel.get().createHistoryPane("Transform");
            auto command = new SelectionCommand(this, historyPane);
            view->GetDocument()->GetCommandProcessor()->Submit(command);
            m_historyPanel.get().AddHistoryItem(view->GetDocument()->GetCommandProcessor(), historyPane);
            view->ResetModified();
        }
    }
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &event)
{
    if (MyApp::GetStrokeSettings().currentTool != ToolType::Text)
    {
        OnMouseUp(event);
    }
}

void DrawingCanvas::OnScroll(wxMouseEvent &event)
{
    if (MyApp::GetStrokeSettings().currentTool == ToolType::ZoomIn)
    {
        if (m_zoomLevel == 0)
        {
            m_lastZoomCenter = event.GetPosition();
        }

        m_zoomLevel = event.GetWheelRotation() / event.GetWheelDelta();

        m_zoomFactor *= std::pow(1.1, m_zoomLevel);

        if (m_zoomFactor < 0.5 || m_zoomFactor > 16)
        {
            return;
        }

        m_zoomToolMenu->CallUpdateZoomComboBox(m_zoomFactor);
        view->SetZoomFactor(m_zoomFactor, event.GetPosition());
    }
    Refresh();
}

void DrawingCanvas::OnKeyDown(wxKeyEvent &event)
{
    if (MyApp::GetStrokeSettings().currentTool == ToolType::Text || MyApp::GetStrokeSettings().currentTool == ToolType::Transform)
    {
        if (event.GetKeyCode() == WXK_DELETE)
        {
            if (view->GetIsSelected())
            {
                HistoryPane *historyPane = m_historyPanel.get().createHistoryPane("Delete");
                auto command = new DeleteCommand(this, historyPane);
                view->GetDocument()->GetCommandProcessor()->Submit(command);
                m_historyPanel.get().AddHistoryItem(view->GetDocument()->GetCommandProcessor(), historyPane);
            }
        }
        else
            view->OnKeyDown(event);
        Refresh();
    }
    event.Skip();
}

void DrawingCanvas::HandleEvent(wxMouseEvent &event)
{
    if (event.GetEventType() == wxEVT_LEFT_DOWN)
    {
        OnMouseDown(event);
    }
    else if (event.GetEventType() == wxEVT_MOTION)
    {
        OnMouseMove(event);
    }
    else if (event.GetEventType() == wxEVT_LEFT_UP)
    {
        OnMouseUp(event);
    }
    else if (event.GetEventType() == wxEVT_LEAVE_WINDOW)
    {
        OnMouseLeave(event);
    }
}

wxString DrawingCanvas::getShapeCommandName()
{
    switch (MyApp::GetStrokeSettings().currentShape)
    {
    case ShapeType::Rect:
        return "Rectangle";
    case ShapeType::Circle:
        return "Circle";
    case ShapeType::Diamond:
        return "Diamond";
    case ShapeType::ITriangle:
        return "ITriangle";
    case ShapeType::RTriangle:
        return "RTriangle";
    case ShapeType::Text:
        return "Text";
    case ShapeType::Path:
        return "Path";
    default:
        return "Shape";
    }
}

void DrawingCanvas::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);

    if (view)
    {
        view->SetVirtualSize(this->GetSize());
        view->SetCanvasBound(wxRect(0, 0, m_initialSize.x, m_initialSize.y));
        view->OnDraw(&dc);
    }
}

void DrawingCanvas::SetView(DrawingView *view)
{
    this->view = view;
    Refresh();
}
