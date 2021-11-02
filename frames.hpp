#pragma once

#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/statbox.h>

class WrappingText : public wxStaticText {
protected:
	wxString shadow_label;
public:
	WrappingText(wxWindow *parent, wxWindowID id, const wxString &label, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxControlNameStr);
	void UpdateLabel(const wxString &label);
	void Wrap(int width);
};

class StepSizer : public wxStaticBoxSizer {
public:
	StepSizer(wxStaticBox *box, int orient);
	StepSizer(int orient, wxWindow *parent, const wxString &label = wxEmptyString);
	void SetLabel(const wxString &label);
};
