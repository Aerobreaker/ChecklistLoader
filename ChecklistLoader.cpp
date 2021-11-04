#include "window.hpp"

#include <wx/app.h>
#include <wx/cmdline.h>

#include <string>

/*
* Todo:
* 1. Create icons for toolbar items, add toolbar items to main window
* 2. Create options menu to save stay-on-top state and size (?)
*/

using namespace std;

class MyApp : public wxApp {
    MainFrame *frm;
    wxString fname;
    bool ontop;
public:
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser &parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser &parser);
};

wxIMPLEMENT_APP(MyApp);

static const wxCmdLineEntryDesc cmd_line_opts[] = {
    {wxCMD_LINE_PARAM, "", "", "filename", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
    {wxCMD_LINE_OPTION, "f", "file", "flag specifying the filename to be loaded"},
    {wxCMD_LINE_SWITCH, "t", "top", "force the window to stay on top"},
    {wxCMD_LINE_NONE}
};

bool MyApp::OnInit() {
    if (!wxApp::OnInit()) return false;
    frm = new MainFrame(NULL, wxID_ANY, "Checklist Loader", wxPoint(50, 50), wxSize(232, 85));
    if (!frm) return false;
    if (!fname.empty()) frm->LoadFile(fname);
    if (ontop) {
        wxCommandEvent evt {};
        frm->m_stay_on_top->Check(true);
        frm->OnStayTop(evt);
    }
    frm->Show(true);
    return true;
}

void MyApp::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(cmd_line_opts);
    return wxApp::OnInitCmdLine(parser);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser &parser) {
    if (!parser.Found("f", &fname)) {
        if (parser.GetParamCount() == 1) {
            fname = parser.GetParam(0);
        } else if (parser.GetParamCount() != 0) {
            parser.Usage();
            return false;
        }
    } else {
        if (parser.GetParamCount() != 0) {
            parser.Usage();
            return false;
        }
    }
    ontop = parser.Found("t");
    return true;
}
