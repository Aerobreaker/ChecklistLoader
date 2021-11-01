#include "window.hpp"
#include <wx/app.h>

/*
* Todo:
* 1. Create options menu to save stay-on-top state and size
* 2. Create icons for toolbar items
*/

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame(NULL, wxID_ANY, "Checklist Loader", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
