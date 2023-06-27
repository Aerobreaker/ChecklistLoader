#include "window.hpp"

#include <wx/app.h>
#include <wx/cmdline.h>

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

/*
//////////////////////////////////////////////////////////////////////////////
//To prepare wxWidgets (for visual studio)                                  //
//////////////////////////////////////////////////////////////////////////////
    1. Go to https://www.wxwidgets.org/downloads/ and download the appropriate
       version.  Installer is preferred
    2. Install somewhere other than under C:\Program Files
    3. (Optional) Create an environment variable (WXWIN) which maps to the
       install directory
    4. Open a visual studio command prompt (either by start menu shortcut or
       by opening visual studio and navigating to Tools->Command Line->
       Developer Command Prompt)
    5. Run the following commands:
            For static libraries (no need to ship libraries, but larger file):
                cd %wxwin%\build\msw && nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=debug CFG=_debug
                cd %wxwin%\build\msw && nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=release CFG=_release
                cd %wxwin%\build\msw && "%vcinstalldir%auxiliary\build\vcvars64.bat" && nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=debug CFG=_debug
                cd %wxwin%\build\msw && "%vcinstalldir%auxiliary\build\vcvars64.bat" && nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=release CFG=_release
            For dynamic libraries (smaller file, but need to ship libraries):
                cd %wxwin%\build\msw && nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=debug CFG=_debug
                cd %wxwin%\build\msw && nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic BUILD=release CFG=_release
                cd %wxwin%\build\msw && "%vcinstalldir%auxiliary\build\vcvars64.bat" && nmake /a /f makefile.vc SHARED=1 TARGET_CPU=X64 CFG=_debug
                cd %wxwin%\build\msw && "%vcinstalldir%auxiliary\build\vcvars64.bat" && nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=release CFG=_release
            NOTE:
                The location of vcvars64.bat seems to change between visual
                studio versions.  These instructions are written for visual
                studio 2019.

To build all:

cd %wxwin%\build\msw
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=release CFG=_release
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic BUILD=release CFG=_release
"%vcinstalldir%auxiliary\build\vcvars64.bat"
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=release CFG=_release
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=release CFG=_release

*/
