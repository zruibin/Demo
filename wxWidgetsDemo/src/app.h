/*
 * app.h
 *
 * Created by Ruibin.Chow on 2022/10/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef APP_H
#define APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace UI {

class Application final : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() wxOVERRIDE;
};

}

#endif /* !APP_H */
