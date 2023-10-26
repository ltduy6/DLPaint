#include "SizeMenu.h"

SizeMenu::SizeMenu(wxWindow *parent, wxWindowID id, int width, const wxPoint &pos, const wxSize &size)
    : SelectableMenu(parent, id, pos, size), width(width)
{
}

void SizeMenu::DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const
{
    gc->SetPen(wxPen(*wxTRANSPARENT_PEN));
    gc->SetBrush(wxBrush(*wxBLACK_BRUSH));
    wxSize size{FromDIP(15), FromDIP(width)};
    gc->DrawRectangle(rect.GetX() + rect.GetWidth() / 2 - size.GetWidth() / 2,
                      rect.GetY() + rect.GetHeight() / 2 - size.GetHeight() / 2,
                      size.GetWidth(), size.GetHeight());
}
