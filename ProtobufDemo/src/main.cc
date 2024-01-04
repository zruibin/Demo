//
//  main.cc
//  ProtobufDemo
//
//  Created by ruibin.chow on 2024/01/03.
//

#include <iostream>
#include "proto/ModelConfig.pb.h"
#include "proto/Address.pb.h"

int main(int argc, char * argv[]) {
    Address address;
    std::cout << "Init: " << address.IsInitialized() << std::endl;
    address.set_id(999);
    std::cout << "isInit: "  << address.IsInitialized() << std::endl;
    
    auto map = address.mutable_id2name();
    map->operator[](19) = "asdf";
    (*map)[20] = "qwer";
    *(address.add_names()) = "good";
    *(address.add_names()) = "bad";
    
    auto names = address.mutable_names();
    for (auto& name : *names) {
        name += " changed";
        std::cout << name << std::endl;
    }
    std::cout << "ShortDebugString: " << address.ShortDebugString() << std::endl;
    std::cout << "SpceUsed: " << address.SpaceUsedLong() << std::endl;
    address.Clear();
    std::cout << "ShortDebugString2: "  << address.ShortDebugString() << std::endl;
    std::cout << "SpceUsed2: "  << address.SpaceUsedLong() << std::endl;
    
    std::cout << "hello world" << std::endl;
}
