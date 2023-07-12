
set_project("CppService")

add_rules("mode.debug", "mode.release")


add_requires("cmake::cJSON", {system = true, configs = {moduledirs = "/opt/homebrew/lib/cmake"}})


target("CppService")
    
    set_kind("binary")

    includes("src/service2/service2.lua")
    includes("src/service/service.lua")
    add_deps("service2", "service")

    add_files("src/*.cc")
    add_defines("DEBUG")

    -- on_load -> after_load -> on_config -> before_build -> on_build -> after_build

    on_load(function (target)
        print("on_load: " .. target:basename())
        if is_plat("linux", "macosx") then
            target:add("links", "pthread", "m", "dl")
        end
    end)

    before_build(function (target)
        print("before_build: " .. target:basename())
        import("core.base.json")

        local componentPath = "./src/service/"
        local luatable = json.loadfile(componentPath .. "component.json")
        print(luatable)

        print(luatable.name)
        print(luatable.description)
        print(luatable.version)
        print(luatable.homepage)
        print(luatable.author)
        for _, name in pairs(luatable.public) do
            print(componentPath .. name)
        end
        for _, name in pairs(luatable.source) do
            print(componentPath .. name)
        end
        for _, name in pairs(luatable.dependency) do
            print(name)
        end
        for _, name in pairs(luatable.resource) do
            print(componentPath .. name)
        end
        

        for key, name in pairs(luatable.object) do
            print(key .. " -> " .. name)
        end

        -- 导入task模块
        import("core.project.task")
        -- 运行hello任务
        task.run("xmakeTask")
    end)

    -- before_link(function (target)
    --     print("before_link")
    -- end)

    after_build(function (target)
        import("core.project.config")
        local targetfile = target:targetfile()
        os.cp(targetfile, path.join(config.buildir(), path.filename(targetfile)))
        print("after_build: %s", targetfile)


        if is_arch("x86_64") then
            print("x86")
        end

        -- 如果当前平台是armv7, arm64, armv7s, armv7-a
        if is_arch("arm.*") then
            print("arm")
        end
    end)



task("xmakeTask")
    on_run("task")
    -- on_run(function ()
    --     print("xmakeTask on_run")
    -- end)

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

