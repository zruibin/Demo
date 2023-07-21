/*
 * CaseFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef CASEFACTORY_H
#define CASEFACTORY_H

#include <memory>
#include <unordered_map>
#include "BaseCaseFactory.h"
#include "BaseCaseBuilder.h"

class CaseFactory : public BaseCaseFactory
{
public:
    explicit CaseFactory() {
        builderMap_ = std::make_shared<std::unordered_map<const char*, std::shared_ptr<BaseCaseBuilder>>>();
    };
    
public:
    void ConstructBuilders() override;
    
    std::shared_ptr<BaseCaseBuilder> GetBuilder(const char* name) override {
        if (name == NULL || std::strlen(name) == 0) {
            return nullptr;
        }
        auto it = builderMap_->find(name);
        if (it != builderMap_->end()) {
            return it->second;
        }
        return nullptr;
    };
    void UpdateBuilder(std::shared_ptr<BaseCaseBuilder> builder) override {
        if (builder == nullptr) {
            return;
        }
        const char* name = builder->GetCaseName();
        auto it = builderMap_->find(name);
        if (it != builderMap_->end()) {
            it->second = builder;
        } else {
            builderMap_->emplace(name, builder);
        }
    };
    
private:
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseCaseBuilder, T>>>
    void AddBuilder() {
        std::shared_ptr<BaseCaseBuilder> builder = std::make_shared<T>();
        builderMap_->emplace(builder->GetCaseName(), builder);
    }
    
private:
    using BuilderMap = std::shared_ptr<std::unordered_map<const char*, std::shared_ptr<BaseCaseBuilder>>>;
    BuilderMap builderMap_;
};


#endif /* !CASEFACTORY_H */
