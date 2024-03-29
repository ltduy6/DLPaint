#include "HistoryPane.h"

int HistoryPane::currentIndex = 0;

HistoryPane::HistoryPane(wxWindow *parent, wxString name, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : SelectablePane(parent, id, pos, size), name(name)
{
}

HistoryPane::~HistoryPane()
{
}

void HistoryPane::SetActive(bool isActive)
{
    this->isActive = isActive;
    if (isActive)
        currentIndex++;
    else
        currentIndex--;
    Refresh();
}

void HistoryPane::DrawContent(wxGraphicsContext *gc, const wxRect &rect, int roundness) const
{
    wxColour bkColor(wxColour(90, 90, 90));
    wxColour textColor(wxColour(231, 246, 242));
    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);

    if (!isActive)
    {
        bkColor = wxColour(100, 100, 100);
        textColor = wxColour(90, 90, 90);
        font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_LIGHT);
    }

    gc->SetBrush(wxBrush(bkColor));
    gc->SetPen(wxPen(bkColor, 1, wxPENSTYLE_SOLID));
    gc->DrawRoundedRectangle(rect.x, rect.y, rect.width, rect.height, roundness);

    gc->SetFont(font, textColor);
    wxDouble width, height;
    gc->GetTextExtent(name, &width, &height);
    gc->DrawText(_(name), rect.x + 5, rect.y + (rect.height - height) / 2);
}
