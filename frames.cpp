#include "frames.hpp"

WrappingText::WrappingText(wxWindow *parent, wxWindowID id, const wxString& label, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxStaticText(parent, id, label, pos, size, style, name) {
	shadow_label = label;
}

void WrappingText::UpdateLabel(const wxString &label) {
	shadow_label = label;
	SetLabel(label);
}

void WrappingText::Wrap(int width) {
	SetLabel(shadow_label);
	wxStaticText::Wrap(width);
}

StepSizer::StepSizer(wxStaticBox *box, int orient) : wxStaticBoxSizer(box, orient) {}

StepSizer::StepSizer(int orient, wxWindow *parent, const wxString &label) : wxStaticBoxSizer(orient, parent, label) {}

void StepSizer::SetLabel(const wxString &label) {
	GetStaticBox()->SetLabel("Step " + label + ":");
}
