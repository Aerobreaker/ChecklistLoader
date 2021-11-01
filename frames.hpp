#pragma once

#include <wx/stattext.h>

class WrappingText : public wxStaticText {
protected:
	wxString shadow_label;
public:
	WrappingText(wxWindow *parent, wxWindowID id, const wxString &label, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxControlNameStr);
	void UpdateLabel(const wxString &label);
	void Wrap(int width);
};