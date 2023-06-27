#include "window.hpp"
#include "Checklist.hpp"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include <cmath>

constexpr char deflabel[36] = "Please load a checklist to continue";

enum {
    ID_OnTop = wxID_HIGHEST + 1,
    ID_Load,
    ID_Unload,
    ID_Load_Sub,
    ID_Unload_Sub
};

void MainFrame::AdvanceList(wxCommandEvent &evt) {
    step_2_check->SetValue(false);

    if (lists.empty()) return OnLoad(evt);

    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if (cur_ind >= cur_list->size()) return;
    if (cur_ind == 0) main_sizer->Show(row_1_sizer);
    step_1_label->UpdateLabel((*cur_list)[cur_ind]->value);
    row_1_sizer->SetLabel((*cur_list)[cur_ind]->key);
    if (++cur_ind == cur_list->size()) {
        if (lists.size() < 2) {
            main_sizer->Hide(row_2_sizer);
        } else {
            OnUnSubList(evt);
            return AdvanceList(evt);
        }
    } else {
        step_2_label->UpdateLabel((*cur_list)[cur_ind]->value);
        row_2_sizer->SetLabel((*cur_list)[cur_ind]->key);
        Enable_Sub((*cur_list)[cur_ind]->sublist != nullptr);
        Enable_Notes((*cur_list)[cur_ind]->notes.length() > 0);
    }
    Resize();
}

void MainFrame::RegressList(wxCommandEvent &evt) {
    step_1_check->SetValue(true);

    if (lists.empty()) return OnLoad(evt);

    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if (cur_ind == 0) return;
    if (cur_ind > cur_list->size()) cur_ind = cur_list->size();
    if (cur_ind == cur_list->size()) main_sizer->Show(row_2_sizer);
    step_2_label->UpdateLabel((*cur_list)[--cur_ind]->value);
    row_2_sizer->SetLabel((*cur_list)[cur_ind]->key);
    Enable_Sub((*cur_list)[cur_ind]->sublist != nullptr);
    Enable_Notes((*cur_list)[cur_ind]->notes.length() > 0);
    if (cur_ind) {
        step_1_label->UpdateLabel((*cur_list)[cur_ind - 1]->value);
        row_1_sizer->SetLabel((*cur_list)[cur_ind - 1]->key);
    } else {
        main_sizer->Hide(row_1_sizer);
    }

    Resize();
}

void MainFrame::LoadFile(std::string &fname) {
    if (!lists.empty() || !indexes.empty()) UnLoad();
    std::shared_ptr<Checklist> lst = Checklist::from_file(fname);
    if (!lst) {
        wxMessageBox("Unable to create list from file!", "Checklist Loader", wxOK | wxCENTER, this);
        return;
    }
    lists.push_back(lst);
    indexes.push_back(0);

    step_1_label->UpdateLabel(wxEmptyString);
    row_1_sizer->SetLabel('0');
    step_2_label->UpdateLabel((*lists[0])[0]->value);
    row_2_sizer->SetLabel((*lists[0])[0]->key);
    Enable_Sub((*lists[0])[0]->sublist != nullptr);
    Enable_Notes((*lists[0])[0]->notes.length() > 0);

    Resize();
}

void MainFrame::LoadFile(wxString &fname) {
    std::string tmp = std::move(fname.ToStdString());
    return LoadFile(tmp);
}

void MainFrame::OnLoad(wxCommandEvent &evt) {
    wxFileDialog openFile(this, "Select list file to open", wxEmptyString, wxEmptyString, "All Files (*.*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFile.ShowModal() == wxID_CANCEL) return;

    std::string fname = openFile.GetPath().ToStdString();
    LoadFile(fname);
}

void MainFrame::UnLoad() {
    lists.clear();
    indexes.clear();

    step_1_label->UpdateLabel(deflabel);
    row_1_sizer->SetLabel('0');
    step_2_label->UpdateLabel(deflabel);
    row_2_sizer->SetLabel('1');

    step_1_check->SetValue(true);
    step_2_check->SetValue(false);

    main_sizer->Hide(row_1_sizer);
    main_sizer->Show(row_2_sizer);
    main_sizer->Hide(row_3_sizer);
    Enable_Sub(false);
    Enable_Notes(false);
    SetSize(GetMinSize());

    Resize();
}

void MainFrame::OnUnload(wxCommandEvent &evt) {
    UnLoad();
}

void MainFrame::OnSubList(wxCommandEvent &evt) {
    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if (!((*cur_list)[cur_ind]->sublist)) return;

    lists.push_back((*cur_list)[cur_ind]->sublist);
    indexes.push_back(0);

    step_1_label->UpdateLabel(wxEmptyString);
    row_1_sizer->SetLabel('0');
    step_2_label->UpdateLabel((*lists.back())[0]->value);
    row_2_sizer->SetLabel((*lists.back())[0]->key);
    Enable_Sub((*lists.back())[0]->sublist != nullptr);
    Enable_Notes((*lists.back())[0]->notes.length() > 0);

    main_sizer->Hide(row_1_sizer);

    Resize();
}

void MainFrame::OnUnSubList(wxCommandEvent &evt) {
    if (lists.size() < 2) return;

    OnUnNotes(evt);

    lists.pop_back();
    indexes.pop_back();

    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if (cur_ind == 0) {
        main_sizer->Hide(row_1_sizer);
    } else {
        main_sizer->Show(row_1_sizer);
    }
    step_1_label->UpdateLabel(cur_ind ? (*cur_list)[cur_ind - 1]->value : deflabel);
    row_1_sizer->SetLabel(cur_ind ? (*cur_list)[cur_ind - 1]->key : "0");
    step_2_label->UpdateLabel((*cur_list)[cur_ind]->value);
    row_2_sizer->SetLabel((*cur_list)[cur_ind]->key);
    Enable_Sub((*cur_list)[cur_ind]->sublist != nullptr);
    Enable_Notes((*cur_list)[cur_ind]->notes.length() > 0);
    
    Resize();
}

void MainFrame::OnNotes(wxCommandEvent &evt) {
    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if ((*cur_list)[cur_ind]->notes.length() < 1) return;

    main_sizer->Hide(row_1_sizer);
    main_sizer->Hide(row_2_sizer);
    main_sizer->Show(row_3_sizer);

    notes_label->UpdateLabel((*cur_list)[cur_ind]->notes);
    row_3_sizer->SetLabel((*cur_list)[cur_ind]->key + " Notes");
    
    Resize();
}

void MainFrame::OnUnNotes(wxCommandEvent &evt) {
    size_t &cur_ind = indexes.back();
    std::shared_ptr<Checklist> cur_list = lists.back();

    if (cur_ind == 0) {
        main_sizer->Hide(row_1_sizer);
    } else {
        main_sizer->Show(row_1_sizer);
    }
    main_sizer->Show(row_2_sizer);
    main_sizer->Hide(row_3_sizer);
    notes_label->UpdateLabel(wxEmptyString);
    row_3_sizer->SetLabel(wxEmptyString);
    Enable_Sub((*cur_list)[cur_ind]->sublist != nullptr);
    Enable_Notes((*cur_list)[cur_ind]->notes.length() > 0);

    Resize();
}

void MainFrame::OnStayTop(wxCommandEvent &evt) {
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

    main_sizer = new wxBoxSizer(wxVERTICAL);

    row_1_sizer = new StepSizer(wxHORIZONTAL, this, "Step 0:");
    step_1_check = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    row_1_sizer->Add(step_1_check, 0, wxALIGN_CENTER | wxALL, 5);
    step_1_label = new SelectableText(this, wxID_ANY, "Please load a checklist to continue", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    row_1_sizer->Add(step_1_label, 1, wxALIGN_CENTER | wxALL, 5);
    main_sizer->Add(row_1_sizer, 1, wxEXPAND, 5);

    row_2_sizer = new StepSizer(wxHORIZONTAL, this, "Step 1:");
    button_sizer = new wxBoxSizer(wxVERTICAL);
    step_2_check = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    row_2_sizer->Add(step_2_check, 0, wxALIGN_CENTER | wxALL, 5);
    step_2_label = new SelectableText(this, wxID_ANY, "Please load a checklist to continue", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    row_2_sizer->Add(step_2_label, 1, wxALIGN_CENTER | wxALL, 5);
    step_2_button = new wxButton(this, wxID_ANY, "Load sub-list", wxDefaultPosition, wxDefaultSize, 0);
    button_sizer->Add(step_2_button, 0, wxALIGN_CENTER | wxALL, 5);
    notes_button = new wxButton(this, wxID_ANY, "Show notes", wxDefaultPosition, wxDefaultSize, 0);
    button_sizer->Add(notes_button, 0, wxALIGN_CENTER | wxALL, 5);
    row_2_sizer->Add(button_sizer, 0, wxALIGN_CENTER | wxALL, 5);
    main_sizer->Add(row_2_sizer, 1, wxEXPAND, 5);

    row_3_sizer = new StepSizer(wxVERTICAL, this, "Notes:");
    notes_label = new SelectableText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    row_3_sizer->Add(notes_label, 1, wxALL | wxEXPAND, 5);
    un_notes_button = new wxButton(this, wxID_ANY, "Return to checklist", wxDefaultPosition, wxDefaultSize, 0);
    row_3_sizer->Add(un_notes_button, 0, wxALIGN_RIGHT | wxALL, 5);
    main_sizer->Add(row_3_sizer, 1, wxEXPAND, 5);

    Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
    step_1_check->Bind(wxEVT_CHECKBOX, &MainFrame::RegressList, this);
    step_2_check->Bind(wxEVT_CHECKBOX, &MainFrame::AdvanceList, this);
    step_2_button->Bind(wxEVT_BUTTON, &MainFrame::OnSubList, this);
    notes_button->Bind(wxEVT_BUTTON, &MainFrame::OnNotes, this);
    un_notes_button->Bind(wxEVT_BUTTON, &MainFrame::OnUnNotes, this);
    Bind(wxEVT_MENU, &MainFrame::OnStayTop, this, ID_OnTop);
    Bind(wxEVT_MENU, &MainFrame::OnLoad, this, ID_Load);
    Bind(wxEVT_MENU, &MainFrame::OnUnload, this, ID_Unload);
    Bind(wxEVT_MENU, &MainFrame::OnSubList, this, ID_Load_Sub);
    Bind(wxEVT_MENU, &MainFrame::OnUnSubList, this, ID_Unload_Sub);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

    SetSizer(main_sizer);
    // Wrap and show/hide rows so that Layout gets the right size here
    step_2_label->Wrap(-1);
    main_sizer->Hide(row_1_sizer);
    main_sizer->Show(row_2_sizer);
    main_sizer->Hide(row_3_sizer);
    // Layout so that the best size gets updated
    Layout();
    // Set the minimum size to the best size with nothing displayed
    SetMinSize(GetBestSize());

    // Now do the rest of the setup that happens when no list is loaded
    UnLoad();

    Centre(wxBOTH);
}

void MainFrame::Enable_Sub(bool enabled) {
    button_sizer->Show(step_2_button, enabled);
    load_sub->Enable(enabled);
    unload_sub->Enable(lists.size() > 1);
}

void MainFrame::Enable_Notes(bool enabled) {
    button_sizer->Show(notes_button, enabled);
}

void MainFrame::OnSize(wxSizeEvent &evt) {
    Layout();
    step_1_label->Wrap(step_1_label->GetClientSize().GetWidth());
    step_2_label->Wrap(step_2_label->GetClientSize().GetWidth());
    Refresh();
    evt.Skip();
}

void MainFrame::Resize() {
    Layout();
    wxSize cur_size = GetSize();
    step_1_label->Wrap(step_1_label->GetClientSize().GetWidth());
    step_2_label->Wrap(step_2_label->GetClientSize().GetWidth());
    wxSize bst_size = GetBestSize();
    /* Scale the size to meet the new area while maintaining aspect ratio
    * New width and height might be off by a couple pixels due to rounding
    * ow=old_wid, oh=old_hgt, nh=new_hgt, tw=target_wid, th=target_hgt, oa=old_area, na=new_area
    * ow*oh=oa
    * ow*nh=na
    * tw*th=na
    * ow/oh=tw/th
    *
    * tw=ow*th/oh
    * ow*th^2/oh=na
    * th^2=na*oh/ow
    * th^2=nh*oh
    */
    int old_wid = cur_size.GetWidth();
    int old_hgt = cur_size.GetHeight();
    int new_wid = std::max(bst_size.GetWidth(), old_wid);
    int new_hgt = std::max(bst_size.GetHeight(), old_hgt);
    // This is here to prevent an infinite resizing loop
    unsigned char cnt = 0;
    // Repeat resizing until size doesn't change
    while (++cnt < 32 && (old_wid != new_wid || old_hgt != new_hgt)) {
        // Cast the ints to double to widen before multiplying and dividing to reduce loss of information
        new_hgt = static_cast<int>(ceil(sqrt(static_cast<double>(new_hgt) * static_cast<double>(old_hgt))));
        new_wid = static_cast<int>(ceil(static_cast<double>(old_wid) * static_cast<double>(new_hgt) / static_cast<double>(old_hgt)));
        bst_size.SetWidth(std::max(old_wid, new_wid));
        bst_size.SetHeight(std::max(old_hgt, new_hgt));
        SetSize(bst_size);
        // Re-wrap after resizing
        Layout();
        step_1_label->Wrap(step_1_label->GetClientSize().GetWidth());
        step_2_label->Wrap(step_2_label->GetClientSize().GetWidth());
        bst_size = GetBestSize();
        old_wid = new_wid;
        old_hgt = new_hgt;
        new_wid = std::max(bst_size.GetWidth(), old_wid);
        new_hgt = std::max(bst_size.GetHeight(), old_hgt);
    }
    Refresh();
    // For some reason, on the next drawing cycle, sometimes the static texts won't be rendered in the vertical center of their horizontal sizer (despite having wxEXPAND set)
    // But if we re-Layout() and invalidate the window and queue the paint request after forcing a draw cycle, it always works
    Update();
    Layout();
    Refresh();
}

void MainFrame::OnExit(wxCommandEvent &evt) {
    Close(true);
}

MainFrame::~MainFrame() {}
