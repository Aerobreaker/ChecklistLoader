#include "window.hpp"
#include <wx/app.h>

/*
* Todo:
* 1. Read filename from command line and, if possible, load on open
* 2. Create options menu to save stay-on-top state and size
* 3. Create icons for toolbar items
*/

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame(NULL, wxID_ANY, "Checklist Loader", wxPoint(50, 50), wxSize(232, 85));
    frame->Show(true);
    return true;
}
