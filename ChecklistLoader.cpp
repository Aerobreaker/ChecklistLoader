#include "window.hpp"

#include <wx/app.h>
#include <wx/cmdline.h>

#include <string>

/*
* Todo:
* 1. Roll custon command-line parser to take arguments either positionally or by name
* 2. Create options menu to save stay-on-top state and size
* 3. Create icons for toolbar items
*/

using namespace std;

class MyApp : public wxApp {
    MainFrame *frm;
public:
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser &parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser &parser);
};

wxIMPLEMENT_APP(MyApp);

static const wxCmdLineEntryDesc cmd_line_opts[] = {
    {wxCMD_LINE_PARAM, "", "", "the filename to be loaded initially (positional)", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
    {wxCMD_LINE_OPTION, "f", "file", "the filename to be loaded (by flag)"},
    {wxCMD_LINE_NONE}
};

bool MyApp::OnInit() {
    frm = new MainFrame(NULL, wxID_ANY, "Checklist Loader", wxPoint(50, 50), wxSize(232, 85));
    if (!wxApp::OnInit()) return false;
    if (!frm) return false;
    frm->Show(true);
    return true;
}

void MyApp::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(cmd_line_opts);
    return wxApp::OnInitCmdLine(parser);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser &parser) {
    wxString tmp;
    if (!parser.Found("f", &tmp)) {
        if (parser.GetParamCount() == 1) {
            frm->LoadFile(parser.GetParam(0));
        } else if (parser.GetParamCount() != 0) {
            parser.Usage();
            return false;
        }
    } else {
        frm->LoadFile(tmp);
    }
    return true;
}
