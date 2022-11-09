/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/10/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include <iostream>
#include "app.h"

//int main(int argc, char *argv[]) {
//    std::cout << "Hello world." << std::endl;
//
//    UI::AppLication app;
//    app.run(argc, argv);
//
//    return 0;
//}

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(UI::Application);



