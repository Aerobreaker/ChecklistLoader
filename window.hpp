#pragma once

#include "Checklist.hpp"
#include "frames.hpp"

#include <wx/toolbar.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>

#include <string>

class MainFrame : public wxFrame {
private:

protected:
    /*
    wxToolBar* main_toolbar;
    wxToolBarToolBase* load_list;
    wxToolBarToolBase* unload_list;
    wxToolBarToolBase* view_sublist;
    wxToolBarToolBase* view_parent_list;
    */
    wxBoxSizer *main_sizer;
    StepSizer *row_1_sizer;
    StepSizer *row_2_sizer;
    StepSizer *row_3_sizer;
    wxBoxSizer *button_sizer;
    wxCheckBox *step_1_check;
    SelectableText *step_1_label;
    wxCheckBox *step_2_check;
    SelectableText *step_2_label;
    wxButton *step_2_button;
    SelectableText *notes_label;
    wxButton *notes_button;
    wxButton *un_notes_button;

    std::vector<std::shared_ptr<Checklist>> lists {};
    std::vector<size_t> indexes {};

public:
    wxMenuItem *m_stay_on_top;
    wxMenuItem *load_sub;
    wxMenuItem *unload_sub;

    MainFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = "Checklist Loader", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

    void OnStayTop(wxCommandEvent &evt);
    void OnLoad(wxCommandEvent &evt);
    void OnUnload(wxCommandEvent &evt);
    void OnSubList(wxCommandEvent &evt);
    void OnUnSubList(wxCommandEvent &evt);
    void OnNotes(wxCommandEvent &evt);
    void OnUnNotes(wxCommandEvent &evt);
    void AdvanceList(wxCommandEvent &evt);
    void RegressList(wxCommandEvent &evt);
    void OnExit(wxCommandEvent &evt);
    void OnSize(wxSizeEvent &evt);

    void LoadFile(std::string &fname);
    void LoadFile(wxString &fname);
    void UnLoad();
    void Resize();
    void Enable_Sub(bool enabled);
    void Enable_Notes(bool enabled);

    ~MainFrame();

    MainFrame(MainFrame &) = delete;
    MainFrame(MainFrame &&) = delete;
    MainFrame operator=(MainFrame) = delete;
    MainFrame &operator=(MainFrame &) = delete;
};
