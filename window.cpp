#include "window.hpp"
#include "Checklist.hpp"

#include <wx\filedlg.h>

constexpr char deflabel[36] = "Please load a checklist to continue";

enum {
    ID_OnTop = wxID_HIGHEST + 1,
    ID_Load,
    ID_Unload,
    ID_Load_Sub,
    ID_Unload_Sub
};

void MainFrame::AdvanceList(wxCommandEvent &evt) {
    evt.Skip();
    step_2_check->SetValue(false);

    if (lists.empty()) return OnLoad(evt);

    size_t &cur_ind = indexes.back();
    Checklist &cur_list = lists.back();

    if (cur_ind >= cur_list.size()) return;
    if (cur_ind == 0) main_sizer->Show(row_1_sizer);
    step_1_label->SetLabel(cur_list[cur_ind]->value);
    if (++cur_ind == cur_list.size()) {
        if (lists.size() < 2) {
            main_sizer->Hide(row_2_sizer);
        } else {
            OnUnSubList(evt);
            return AdvanceList(evt);
        }
    } else {
        step_2_label->SetLabel(cur_list[cur_ind]->value);
        Enable_Sub(cur_list[cur_ind]->sublist);
    }
    Resize();
}

void MainFrame::RegressList(wxCommandEvent &evt) {
    evt.Skip();
    step_1_check->SetValue(true);

    if (lists.empty()) return OnLoad(evt);

    size_t &cur_ind = indexes.back();
    Checklist &cur_list = lists.back();

    if (cur_ind == 0) return;
    if (cur_ind > cur_list.size()) cur_ind = cur_list.size();
    if (cur_ind == cur_list.size()) main_sizer->Show(row_2_sizer);
    step_2_label->SetLabel(cur_list[--cur_ind]->value);
    Enable_Sub(cur_list[cur_ind]->sublist);
    if (cur_ind) {
        step_1_label->SetLabel(cur_list[cur_ind - 1]->value);
    } else {
        main_sizer->Hide(row_1_sizer);
    }
}

void MainFrame::OnLoad(wxCommandEvent &evt) {
    if (!lists.empty() || !indexes.empty()) OnUnload(evt);

    wxFileDialog openFile(this, "Select list file to open", wxEmptyString, wxEmptyString, "Any file|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFile.ShowModal() == wxID_CANCEL) return;

    lists.push_back(Checklist::from_file(openFile.GetPath().ToStdString()));
    indexes.push_back(0);

    step_1_label->SetLabel(wxEmptyString);
    step_2_label->SetLabel(lists[0][0]->value);
    Enable_Sub(lists[0][0]->sublist);

    Resize();
}

void MainFrame::OnUnload(wxCommandEvent &evt) {
    lists.clear();
    indexes.clear();

    step_1_label->SetLabel(deflabel);
    step_2_label->SetLabel(deflabel);

    step_1_check->SetValue(true);
    step_2_check->SetValue(false);

    main_sizer->Hide(row_1_sizer);
    main_sizer->Show(row_2_sizer);
    Enable_Sub(false);

    Resize();
}

void MainFrame::OnSubList(wxCommandEvent &evt) {
    size_t &cur_ind = indexes.back();
    Checklist &cur_list = lists.back();

    if (!(cur_list[cur_ind]->sublist)) return;

    lists.push_back(*(cur_list[cur_ind]->sublist));
    indexes.push_back(0);

    step_1_label->SetLabel(wxEmptyString);
    step_2_label->SetLabel(lists.back()[0]->value);
    Enable_Sub(lists.back()[0]->sublist);

    Resize();
}

void MainFrame::OnUnSubList(wxCommandEvent &evt) {
    if (lists.size() < 2) return;

    lists.pop_back();
    indexes.pop_back();

    size_t &cur_ind = indexes.back();
    Checklist &cur_list = lists.back();
    Node *tmp;

    if (cur_ind == 0) main_sizer->Hide(row_1_sizer);
    if (cur_ind) tmp = cur_list[cur_ind - 1];
    step_1_label->SetLabel(cur_ind ? cur_list[cur_ind - 1]->value : deflabel);
    tmp = cur_list[cur_ind];
    step_2_label->SetLabel(cur_list[cur_ind]->value);
    Enable_Sub(cur_list[cur_ind]->sublist);
    
    Resize();
}

void MainFrame::OnStayTop(wxCommandEvent &evt) {
    evt.Skip();
    long cur_style = GetWindowStyle(), new_style = 0;

    if (m_stay_on_top->IsChecked()) {
        new_style = cur_style | wxSTAY_ON_TOP;
    } else {
        new_style = cur_style & ~wxSTAY_ON_TOP;
    }

    SetWindowStyle(new_style);
}

MainFrame::MainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) : wxFrame(parent, id, title, pos, size, style) {
    SetSizeHints(wxDefaultSize, wxDefaultSize);

    /*Menu accelerators:
    * F - File menu
    * L - List menu
    */
    wxMenuBar *main_menu = new wxMenuBar(0);
    /*Menu accelerators:
    * S - Stay on top
    * Q - Quit
    */
    wxMenu *file_menu = new wxMenu();
    m_stay_on_top = new wxMenuItem(file_menu, ID_OnTop, "&Stay on top", wxEmptyString, wxITEM_CHECK);
    wxMenuItem *m_quit = new wxMenuItem(file_menu, wxID_EXIT);
    file_menu->Append(m_stay_on_top);
    file_menu->Append(m_quit);
    main_menu->Append(file_menu, "&File");

    /*Menu accelerators:
    * L - Load checklist
    * U - Unload checklist
    * V - Load sub-list
    * P - Unload sub-list
    */
    wxMenu *list_menu = new wxMenu();
    wxMenuItem *load = new wxMenuItem(list_menu, ID_Load, "&Load checklist");
    wxMenuItem *unload = new wxMenuItem(list_menu, ID_Unload, "&Unload checklist");
    load_sub = new wxMenuItem(list_menu, ID_Load_Sub, "&View sub-list");
    unload_sub = new wxMenuItem(list_menu, ID_Unload_Sub, "View &parent list");
    list_menu->Append(load);
    list_menu->Append(unload);
    list_menu->AppendSeparator();
    list_menu->Append(load_sub);
    list_menu->Append(unload_sub);
    main_menu->Append(list_menu, "&List");

    SetMenuBar(main_menu);

    /*
     * Need to add some bitmaps for the toolbar buttons in order to use them
    main_toolbar = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
    load_list = main_toolbar->AddTool(wxID_ANY, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

    unload_list = main_toolbar->AddTool(wxID_ANY, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

    main_toolbar->AddSeparator();

    view_sublist = main_toolbar->AddTool(wxID_ANY, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

    view_parent_list = main_toolbar->AddTool(wxID_ANY, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

    main_toolbar->Realize();
     */

    /*
    * Test
    */
    main_sizer = new wxBoxSizer(wxVERTICAL);

    row_1_sizer = new wxBoxSizer(wxHORIZONTAL);
    step_1_check = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    row_1_sizer->Add(step_1_check, 0, wxALIGN_CENTER | wxALL, 5);
    step_1_label = new wxStaticText(this, wxID_ANY, "Please load a checklist to continue", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    row_1_sizer->Add(step_1_label, 1, wxALIGN_CENTER | wxALL, 5);
    main_sizer->Add(row_1_sizer, 1, wxEXPAND, 5);

    row_2_sizer = new wxBoxSizer(wxHORIZONTAL);
    step_2_check = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    row_2_sizer->Add(step_2_check, 0, wxALIGN_CENTER | wxALL, 5);
    step_2_label = new wxStaticText(this, wxID_ANY, "Please load a checklist to continue", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    row_2_sizer->Add(step_2_label, 1, wxALIGN_CENTER | wxALL, 5);
    step_2_button = new wxButton(this, wxID_ANY, "Load sub-list", wxDefaultPosition, wxDefaultSize, 0);
    row_2_sizer->Add(step_2_button, 0, wxALIGN_CENTER | wxALL, 5);
    main_sizer->Add(row_2_sizer, 1, wxEXPAND, 5);

    /*
     * Bindings go here
     * Bind({event_type}, &Function (pass function by reference, or lambda function), this[, {ID_whitelist});
     */
    step_1_check->Bind(wxEVT_CHECKBOX, &MainFrame::RegressList, this);
    step_2_check->Bind(wxEVT_CHECKBOX, &MainFrame::AdvanceList, this);
    step_2_button->Bind(wxEVT_BUTTON, &MainFrame::OnSubList, this);

    Bind(wxEVT_MENU, &MainFrame::OnStayTop, this, ID_OnTop);
    Bind(wxEVT_MENU, &MainFrame::OnLoad, this, ID_Load);
    Bind(wxEVT_MENU, &MainFrame::OnUnload, this, ID_Unload);
    Bind(wxEVT_MENU, &MainFrame::OnSubList, this, ID_Load_Sub);
    Bind(wxEVT_MENU, &MainFrame::OnUnSubList, this, ID_Unload_Sub);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

    SetSizer(main_sizer);
    step_1_label->Wrap(-1);
    step_2_label->Wrap(-1);
    step_1_check->SetValue(true);
    main_sizer->Hide(row_1_sizer);
    main_sizer->Show(row_2_sizer);
    Enable_Sub(false);
    Layout();
    SetSize(GetBestSize());

    Centre(wxBOTH);
}

void MainFrame::Enable_Sub(bool enabled) {
    row_2_sizer->Show(step_2_button, enabled);
    load_sub->Enable(enabled);
    unload_sub->Enable(lists.size() > 1);
}

void MainFrame::Resize() {
    step_1_label->Wrap(-1);
    step_2_label->Wrap(-1);
    Layout();
    wxSize cur_size = GetSize();
    wxSize bst_size = GetBestSize();
    if (cur_size.GetWidth() < bst_size.GetWidth() || cur_size.GetHeight() < bst_size.GetHeight()) SetSize(bst_size);
}

void MainFrame::OnExit(wxCommandEvent &evt) {
    Close(true);
}

MainFrame::~MainFrame() {}
