--! /usr/bin/env lua
--
-- components.lua
--
-- Created by Ruibin.Chow on 2023/07/14.
-- Copyright (c) 2023年 Ruibin.Chow All rights reserved.
--


target("Case")
    set_kind("static")

    if is_mode("debug") then
        -- add_defines("DEBUG")
    elseif is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    add_cxxflags("-stdlib=libc++", {tools = "clang"})
    set_languages("c99", "cxx17")

    add_includedirs("base", {public = true})
    add_headerfiles("base/*.h", {install = true})


    add_includedirs(os.dirs(path.join(os.scriptdir(), "components/**/include")), {public = true})
    add_includedirs(os.dirs(path.join(os.scriptdir(), "components/**")))
    add_files("components/**.cc")
    add_files("components/**.cpp")
    add_files("components/**.hpp")
    add_files("components/**.c")
    add_headerfiles("components/**/include/*.hpp", {install = true})
    add_headerfiles("components/**/include/*.h", {install = true})


    add_includedirs("shell")
    add_includedirs(os.dirs(path.join(os.scriptdir(), "shell/**")))
    add_files("shell/**.c")
    add_files("shell/**.cc")
    -- add_files("shell/**.cpp")
    -- add_files("shell/**.hpp")
    add_headerfiles("shell/**.hpp", {install = false})
    add_headerfiles("shell/**.h", {install = false})

    
    add_headerfiles("CaseEngine.h", {install = true})
    add_files("CaseEngine.cc")

    add_deps("Service")
