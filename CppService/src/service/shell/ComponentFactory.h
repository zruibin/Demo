/*
 * ComponentFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <memory>
#include <unordered_map>
#include "BaseServiceFactory.h"
#include "BaseServiceBuilder.h"

class ComponentFactory : public BaseServiceFactory
{
public:
    explicit ComponentFactory() {
        builderMap_ = std::make_shared<std::unordered_map<const char*, std::shared_ptr<BaseServiceBuilder>>>();
    };
    
public:
    void ConstructBuilders() override;
    
    std::shared_ptr<BaseServiceBuilder> GetBuilder(const char* name) override {
        if (name == NULL || std::strlen(name) == 0) {
            return nullptr;
        }
        auto it = builderMap_->find(name);
        if (it != builderMap_->end()) {
            return it->second;
        }
        return nullptr;
    };
    void UpdateBuilder(std::shared_ptr<BaseServiceBuilder> builder) override {
        if (builder == nullptr) {
            return;
        }
        const char* name = builder->GetServiceName();
        auto it = builderMap_->find(name);
        if (it != builderMap_->end()) {
            it->second = builder;
        } else {
            builderMap_->emplace(name, builder);
        }
    };
    
private:
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseServiceBuilder, T>>>
    void AddBuilder() {
        std::shared_ptr<BaseServiceBuilder> builder = std::make_shared<T>();
        builderMap_->emplace(builder->GetServiceName(), builder);
    }
    
private:
    using BuilderMap = std::shared_ptr<std::unordered_map<const char*, std::shared_ptr<BaseServiceBuilder>>>;
    BuilderMap builderMap_;
};


#endif /* !COMPONENTFACTORY_H */
