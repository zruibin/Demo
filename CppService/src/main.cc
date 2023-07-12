#include <stdio.h>

#include <memory>
#include "Engine.h"
#include "ServiceInterface.h"
#include "AudioDectInterface.h"

int main(int argc, char** argv)
{
    printf("hello world!\n");
    
    std::shared_ptr<Engine> engine = std::make_shared<Engine>();
    engine->Init();
    
    std::shared_ptr<ServiceInterface> service = engine->GetService<ServiceInterface>(Protocol(ServiceInterface));
    if (service != nullptr) {
        service->Description();
        service->Test();
    }
    
    auto audioDectService = engine->GetService<AudioDectInterface>(Protocol(AudioDectInterface));
    if (audioDectService != nullptr) {
        audioDectService->Description();
    }
    
    engine->Destory();
    return 0;
}
