/*
 *
 * CaseEngine.cc
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "CaseEngine.h"
#include "BaseCase.h"
#include "CaseFactory.h"

CaseEngine::CaseEngine() {
    caseMap_ = std::make_shared<std::unordered_map<std::string,
                                       std::shared_ptr<BaseCase>>>();
    caseFactory_ =  std::make_shared<CaseFactory>();
}

CaseEngine::~CaseEngine() {
    
}

std::shared_ptr<BaseCase> CaseEngine::GetCaseByName(const std::string& name) {
    if (name.length() == 0) {
        return nullptr;
    }
    auto caseIter = caseMap_->find(name);
    if (caseIter != caseMap_->end()) {
        return caseIter->second;
    } else {
        auto builder = caseFactory_->GetBuilder(name);
        if (builder != nullptr) {
            builder->SetServiceEngine(serviceEngine_.lock());
            builder->SetCaseEngine(this->shared_from_this());
            auto caseObj = builder->BuildCase();
            caseObj->Init();
            caseMap_->emplace(name, caseObj);
            return caseObj;
        }
    }
    return nullptr;
}

std::shared_ptr<BaseCase> CaseEngine::CreateCaseByName(const std::string& name) {
    if (name.length() == 0) {
        return nullptr;
    }
    auto builder = caseFactory_->GetBuilder(name);
    if (builder != nullptr) {
        builder->SetCaseEngine(this->shared_from_this());
        auto caseObj = builder->BuildCase();
        return caseObj;
    }
    return nullptr;
}

void CaseEngine::Init(void) {
    caseFactory_->ConstructBuilders();
}

void CaseEngine::Destory(void) {
    if (caseMap_ != nullptr) {
        for (auto& [key, caseObj] : *caseMap_) {
            caseObj->Destory();
        }
        caseMap_->clear();
        caseMap_ = nullptr;
    }
}

void CaseEngine::UpdateCase(std::shared_ptr<BaseCaseBuilder> builder) {
    if (builder != nullptr) {
        caseFactory_->UpdateBuilder(builder);
    }
}

