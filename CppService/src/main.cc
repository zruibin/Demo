#include <stdio.h>

#include <memory>
#include <iostream>
#include "ServiceEngine.h"
#include "LoggerInterface.h"
#include "ServiceInterface.h"
#include "AudioDectInterface.h"
#include "CaseEngine.h"
#include "TestCaseInterface.h"

int main(int argc, char** argv)
{
    printf("hello world!\n");
    
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
    return 0;
}
