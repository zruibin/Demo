#include <stdio.h>

#include <memory>
#include <iostream>
#include "ServiceEngine.h"
#include "LoggerInterface.h"
#include "ServiceInterface.h"
#include "AudioDectInterface.h"

int main(int argc, char** argv)
{
    printf("hello world!\n");
    
    std::shared_ptr<BaseServiceEngine> engine = std::make_shared<ServiceEngine>();
    engine->Init();
    
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
    
    engine->Destory();
    return 0;
}
