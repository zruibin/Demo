/*
 * CaseFactory.cc
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "CaseFactory.h"
#include "TestCaseBuilder.hpp"

void CaseFactory::ConstructBuilders() {
    AddBuilder<TestCaseBuilder>();
}
