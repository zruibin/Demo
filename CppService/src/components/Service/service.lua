--
-- service.lua
--
-- Created by Ruibin.Chow on 2023/06/30.
-- Copyright (c) 2023年 Ruibin.Chow All rights reserved.
--

target("service")
    set_kind("static")

    if is_mode("debug") then
        add_defines("DEBUG")
    elseif is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    set_languages("c++17")

    add_files("source/*.cc")
    add_includedirs("include", {public = true,})
    add_headerfiles("include/*.h", {install = false})

    add_sysincludedirs("../service2/include")
    add_deps("service2")

    -- add_filegroups("serviceGroup", {rootdir = "source/"})
    -- set_group("serviceGroup")

    -- add_files({
    -- 	"component.json",
    -- 	"resource/tt.md"
    -- })