/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/06/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include <iostream>
#include "app.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello world." << std::endl;

    UI::AppLication app;
    app.run(argc, argv);

    return 0;
}


