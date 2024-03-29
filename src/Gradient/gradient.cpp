#include "gradient.h"

wxGradient::wxGradient() {}

wxGradient::wxGradient(wxVector<wxColour> stops, int min, int max)
{
	Create(stops, min, max);
}
bool wxGradient::Create(wxVector<wxColour> stops, int min, int max)
{
	m_stops = stops;
	m_min = min;
	m_max = max;
	return true;
}
wxGradient::~wxGradient() {}
wxColour wxGradient::getColorAt(int value)
{
	if (value < m_min)
	{
		return m_min;
	}
	else if (value > m_max)
	{
		return m_max;
	}
	else
	{
		int range = m_max - m_min;
		int v = value - m_min;
		double step = range / (double)(m_stops.size() - 1);
		int bin = (int)(v / step);
		double normalized_v = (v - bin * step) / step;
		return lerp(m_stops[bin], m_stops[bin + 1], normalized_v);
	}
}
void wxGradient::addColorStop(wxColour col)
{
	m_stops.push_back(col);
}
void wxGradient::insertColorStop(int index, wxColour col)
{
	if (index > (int)m_stops.size())
		addColorStop(col);
	else
		m_stops.insert(m_stops.begin() + index, col);
}
void wxGradient::removeColorStop(int index)
{
	m_stops.erase(m_stops.begin() + index);
}
void wxGradient::editColorStop(int index, wxColor col)
{
	m_stops[index] = col;
}
void wxGradient::setMin(int min)
{
	m_min = min;
}
int wxGradient::getMin()
{
	return m_min;
}
void wxGradient::setMax(int max)
{
	m_max = max;
}
int wxGradient::getMax()
{
	return m_max;
}
void wxGradient::setStops(wxVector<wxColour> stops)
{
	m_stops = stops;
}
wxVector<wxColor> wxGradient::getStops()
{
	return m_stops;
}
void wxGradient::fromString(wxString str)
{
	if (str.length() <= 1)
	{
		addColorStop(wxColour(0, 0, 0));
		addColorStop(wxColour(255, 255, 255));
	}
	while (str.length() > 1)
	{
		addColorStop(wxColour(str.BeforeFirst(';')));
		str = str.AfterFirst(';');
	}
}
wxString wxGradient::toString()
{
	wxVector<wxColour>::iterator itr;
	wxString str = "";
	for (itr = m_stops.begin(); itr != m_stops.end(); ++itr)
	{
		str.Append(itr->GetAsString(wxC2S_CSS_SYNTAX));
		str.Append(';');
	}
	return str;
}
wxColor wxGradient::lerp(wxColor c1, wxColor c2, double value)
{
	unsigned char R = (1.0 - value) * c1.Red() + value * c2.Red();
	unsigned char G = (1.0 - value) * c1.Green() + value * c2.Green();
	unsigned char B = (1.0 - value) * c1.Blue() + value * c2.Blue();
	return wxColor(R, G, B);
}
