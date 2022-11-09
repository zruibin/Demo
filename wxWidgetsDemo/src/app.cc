/*
 *
 * app.cc
 *
 * Created by Ruibin.Chow on 2022/10/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "app.h"
#include <iostream>
#include <absl/strings/str_cat.h>

namespace UI {

enum {
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
        // set the frame icon
    //    SetIcon(wxICON(sample));

    #if wxUSE_MENUBAR
        // create a menu bar
        wxMenu *fileMenu = new wxMenu;

        // the "About" item should be in the help menu
        wxMenu *helpMenu = new wxMenu;
        helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

        fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

        // now append the freshly created menu to the menu bar...
        wxMenuBar *menuBar = new wxMenuBar();
        menuBar->Append(fileMenu, "&File");
        menuBar->Append(helpMenu, "&Help");

        // ... and attach this menu bar to the frame
        SetMenuBar(menuBar);
    #else // !wxUSE_MENUBAR
        // If menus are not available add a button to access the about box
        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
        aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
        sizer->Add(aboutBtn, wxSizerFlags().Center());
        SetSizer(sizer);
    #endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

    #if wxUSE_STATUSBAR
        // create a status bar just for fun (by default with 1 pane only)
        CreateStatusBar(2);
        SetStatusText("Welcome to wxWidgets!");
    #endif // wxUSE_STATUSBAR
    };

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event) {
        Close(true);
    };
    
    void OnAbout(wxCommandEvent& event) {
        wxMessageBox(wxString::Format
        (
           "Welcome to %s!\n"
           "\n"
           "This is the wxWidgets Demo\n"
           "running under %s.",
           wxVERSION_STRING,
           wxGetOsDescription()
        ),
        "About wxWidgets Demo",
        wxOK | wxICON_INFORMATION,
        this);
    };

private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()



// 'Main program' equivalent: the program execution "starts" here
bool Application::OnInit()
{
    std::string s1;
    int i = 255;
    s1 = absl::StrCat("hello 0x", absl::Hex(i));
    std::cout << "abseil: " << s1 << std::endl;
    
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame("wxWidgets App");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}



}

