/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2024/01/20.
 * Copyright (c) 2024年 Ruibin.Chow All rights reserved.
 */

#include <iostream>

extern int testSequential_main(int, char **);
extern int testParallel_main(int, char **);
extern int testImAnim(int, char**);

int main(int argc, char* argv[]) {
    std::cout << "Hello ImGUI Demo." << std::endl;
    testImAnim(argc, NULL);
    return 0;
}
