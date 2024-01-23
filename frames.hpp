#pragma once

#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>

class WrappingText : public wxStaticText {
protected:
	wxString shadow_label;
public:
	WrappingText(wxWindow *parent, wxWindowID id, const wxString &label, long style = 0, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, const wxString &name = wxControlNameStr);
	void UpdateLabel(const wxString &label);
	void Wrap(int width);
	WrappingText(WrappingText &) = delete;
	WrappingText(WrappingText &&) = delete;
	WrappingText operator=(WrappingText) = delete;
	WrappingText &operator=(WrappingText &) = delete;
};

class StepSizer : public wxStaticBoxSizer {
public:
	StepSizer(wxStaticBox *box, int orient);
	StepSizer(int orient, wxWindow *parent, const wxString &label = wxEmptyString);
	void SetLabel(const wxString &label);
	StepSizer(StepSizer &) = delete;
	StepSizer(StepSizer &&) = delete;
	StepSizer operator=(StepSizer) = delete;
	StepSizer &operator=(StepSizer &) = delete;
};

class SelectableText : public wxTextCtrl {
protected:
	WrappingText *shadow_label;
public:
	SelectableText(wxWindow *parent, wxWindowID id, const wxString &text, long style = 0, bool monospace = false, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, const wxString &name = wxControlNameStr, const wxValidator &validator = wxDefaultValidator);
	void UpdateLabel(const wxString &label);
	void Wrap(int width);
	void SetLabel(const wxString &label);
	SelectableText(SelectableText &) = delete;
	SelectableText(SelectableText &&) = delete;
	SelectableText operator=(SelectableText) = delete;
	SelectableText &operator=(SelectableText &) = delete;
};