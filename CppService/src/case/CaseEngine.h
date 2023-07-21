/*
 * CaseEngine.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef CASEENGINE_H
#define CASEENGINE_H

#include <unordered_map>
#include "BaseCaseEngine.h"

class CaseEngine : public BaseCaseEngine
{
public:
    CaseEngine();
    virtual ~CaseEngine();

protected:
    std::shared_ptr<BaseCase> GetCaseByName(const char*) override;
    std::shared_ptr<BaseCase> CreateCaseByName(const char*) override;

public:
    void Init(void) override;
    void Destory(void) override;
    void UpdateCase(std::shared_ptr<BaseCaseBuilder>) override;

private:
    std::shared_ptr<std::unordered_map<const char*,
                                       std::shared_ptr<BaseCase>>> caseMap_;
};

#endif /* !CASEENGINE_H */
