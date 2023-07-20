/*
 * BaseCaseBuilder.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECASEBUILDER_H
#define BASECASEBUILDER_H


#include <string>
#include <memory>

class BaseCase;
class BaseCaseEngine;
class BaseServiceEngine;

class BaseCaseBuilder : public std::enable_shared_from_this<BaseCaseBuilder>
{
public:
    virtual ~BaseCaseBuilder() = default;
    
public:
    virtual std::shared_ptr<BaseCase> BuildCase() = 0;
    virtual std::string GetCaseName() = 0;
    virtual std::string GetBuilderName() = 0;
    virtual void SetCaseEngine(std::shared_ptr<BaseCaseEngine> caseEngine) {
        caseEngine_ = caseEngine;
    }
    virtual void SetServiceEngine(std::shared_ptr<BaseServiceEngine> serviceEngine) {
        serviceEngine_ = serviceEngine;
    }
    
protected:
    std::weak_ptr<BaseCaseEngine> caseEngine_;
    std::weak_ptr<BaseServiceEngine> serviceEngine_;
};

#endif /* !BASECASEBUILDER_H */
