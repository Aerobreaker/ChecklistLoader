#include "frames.hpp"

WrappingText::WrappingText(wxWindow *parent, wxWindowID id, const wxString &label, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxStaticText(parent, id, label, pos, size, style, name) {
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

SelectableText::SelectableText(wxWindow *parent, wxWindowID id, const wxString &text, const wxPoint &pos, const wxSize &size, long style, const wxString &name, const wxValidator &validator) : wxTextCtrl(parent, id, text, pos, size, style | wxTE_READONLY | wxBORDER_NONE, validator, name) {
	shadow_label = new WrappingText(parent, id, text, pos, size, style, name);
	shadow_label->Hide();
	SetSize(shadow_label->GetSize());
	SetFont(parent->GetFont());
	SetForegroundColour(parent->GetForegroundColour());
	SetBackgroundColour(parent->GetBackgroundColour());
}

void SelectableText::UpdateLabel(const wxString &label) {
	SetValue(label);
	shadow_label->UpdateLabel(label);
	SetSize(shadow_label->GetSize());
}

void SelectableText::Wrap(int width) {
	shadow_label->Wrap(width);
	SetSize(shadow_label->GetSize());
}

void SelectableText::SetLabel(const wxString &label) {
	SetValue(label);
	shadow_label->SetLabel(label);
	SetSize(shadow_label->GetSize());
}
