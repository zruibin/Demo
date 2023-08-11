/*
 * thief.hpp
 *
 * Created by Ruibin.Chow on 2023/08/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef THIEF_HPP
#define THIEF_HPP

#include <string>
#include <iostream>

/*
 非侵入式访问对象私有成员
 http://www.purecpp.cn/detail?id=2372
 */

template <typename T, auto... field>
struct ThiefMember {
    friend auto steal_impl(T& t) {
        return std::make_tuple(field...);
    }
};

class Bank_t {
    int id;
    std::string name;

    int foo(){
        return 42 + id;
    }

public:
    Bank_t(int i, std::string str) : id(i), name(str){}
};

auto steal_impl(Bank_t& t);
template struct ThiefMember<Bank_t, &Bank_t::id, &Bank_t::name, &Bank_t::foo>;


int thief_main(void) {
    std::cout << "----------------------------------------------------------" << std::endl;
    Bank_t bank(1, "ok");
    auto tp = steal_impl(bank);

    auto id = bank.*(std::get<0>(tp)); // 1
    std::cout << "id: " << id << std::endl;
    
    auto name = bank.*(std::get<1>(tp)); //ok
    std::cout << "name: " << name << std::endl;

    auto r = (bank.*(std::get<2>(tp)))(); // 42
    std::cout << "foo: " << r << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
}

#endif /* !THIEF_HPP */
