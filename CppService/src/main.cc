#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include "ServiceEngine.h"
#include "LoggerInterface.h"
#include "ServiceInterface.h"
#include "AudioDectInterface.h"
#include "CaseEngine.h"
#include "TestCaseInterface.h"
#include "thief.hpp"

#define MEMORY_COUNT 1

#if MEMORY_COUNT
int handler = [] () {
    std::cout << "Lambda handler..." << std::endl;
    return 0;
}();

static int memoryCount = 0;

void* operator new(std::size_t size) noexcept(false) {
//    std::cout << "new: " << size << std::endl;
    if (size == 0) {
        size = 1;
    }
    void* p;
    while ((p = ::malloc(size)) == 0) { //采用 malloc 分配空间
        std::new_handler nh = std::get_new_handler();
        if (nh)
            nh();
        else
            throw std::bad_alloc();
    }
    ++memoryCount;
    return p;
}

void operator delete(void* const block) noexcept {
//    std::cout << "delete." << std::endl;
    // 单例对象、静态new对象除外
    free(block);
    --memoryCount;
}

/*
void* operator new[](std::size_t size) noexcept(false) {
    std::cout << "new: " << size << std::endl;
    if (size == 0) {
        size = 1;
    }
    void* p;
    while ((p = ::malloc(size)) == 0) { //采用 malloc 分配空间
        std::new_handler nh = std::get_new_handler();
        if (nh)
            nh();
        else
            throw std::bad_alloc();
    }
    ++memoryCount;
    return p;
}

void operator delete[](void* const block) noexcept {
    std::cout << "delete[]." << std::endl;
    // 单例对象、静态new对象除外
    free(block);
    --memoryCount;
}
 //*/

void exitFun(void) {
    std::cout << "exitFun." << std::endl;
}

__attribute__((constructor)) void load_file() {
    std::cout << "Constructor is called." << std::endl;
    std::cout << "memoryCount: " << memoryCount << std::endl;
}

__attribute__((destructor)) void unload_file() {
    std::cout << "Destructor is called." << std::endl;
    std::cout << "memoryCount: " << memoryCount << std::endl;
}

void test() {
    std::cout << "----------------------------------------------------------" << std::endl;
    int* a = new int;
    *a = 10;
    std::cout << *a << std::endl;
    std::cout << "memoryCount1: " << memoryCount << std::endl;
    delete a;
    std::cout << "memoryCount1: " << memoryCount << std::endl;
    
    int* b = new int[10];
    b[1] = 2;
    std::cout << b[1] << std::endl;
    std::cout << "memoryCount3: " << memoryCount << std::endl;
    delete[] b;
    std::cout << "memoryCount4: " << memoryCount << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
}

#endif

int main(int argc, char** argv)
{
    printf("hello world!\n");
    thief_main();
    
#if MEMORY_COUNT
    test();
    atexit(exitFun);
#endif
    
    //*
    std::shared_ptr<BaseServiceEngine> engine = std::make_shared<ServiceEngine>();
    engine->Init();
    
    std::shared_ptr<BaseCaseEngine> caseEngine = std::make_shared<CaseEngine>();
    caseEngine->SetServiceEngine(engine);
    caseEngine->Init();
    
    std::shared_ptr<LoggerInterface> loggerService = engine->GetService<LoggerInterface>(Protocol(LoggerInterface));
    if (loggerService != nullptr) {
        loggerService->SetMinSeverity(LoggingSeverity::VERBOSE);
        loggerService->InjectLoggerHanlder([](LoggingSeverity, const char* string) {
            std::cout << string;
        });
        std::cout << loggerService->Description() << std::endl;
    }
    
    std::shared_ptr<ServiceInterface> service = engine->GetService<ServiceInterface>(Protocol(ServiceInterface));
    if (service != nullptr) {
        service->Description();
        service->Test();
    }
    
    auto audioDectService = engine->GetService<AudioDectInterface>(Protocol(AudioDectInterface));
    if (audioDectService != nullptr) {
        audioDectService->Description();
    }
    
    std::cout << "----------------------------------------------------------" << std::endl;
    {
        std::shared_ptr<ServiceInterface> tmpService = engine->CreateService<ServiceInterface>(Protocol(ServiceInterface));
        if (tmpService != nullptr) {
            tmpService->Init();
            tmpService->Description();
            tmpService->Test();
            tmpService->Destory();
        }
    }
    std::cout << "----------------------------------------------------------" << std::endl;
    
    
    auto testCase = caseEngine->GetCase<TestCaseInterface>(Protocol(TestCaseInterface));
    if (testCase != nullptr) {
        std::cout << testCase->Description() << std::endl;
    }
    
    std::cout << "----------------------------------------------------------" << std::endl;
    {
        std::shared_ptr<TestCaseInterface> tmpCase = caseEngine->CreateCase<TestCaseInterface>(Protocol(TestCaseInterface));
        if (tmpCase != nullptr) {
            tmpCase->Init();
            tmpCase->Description();
            tmpCase->Destory();
        }
    }
    std::cout << "----------------------------------------------------------" << std::endl;
    
    caseEngine->Destory();
    engine->Destory();
    //*/
    
    return 0;
}
