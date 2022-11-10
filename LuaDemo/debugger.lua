#! /usr/bin/env lua
--
-- debugger.lua
--
-- Created by Ruibin.Chow on 2022/11/10.
-- Copyright (c) 2022年 Ruibin.Chow All rights reserved.
--

--[[
https://zhuanlan.zhihu.com/p/76250440
--]]

-- 调试器后台，处理具体的命令
local dbgd = {
    cmdlist = {},   -- 将执行的命令列表
    info = {},      -- 当前的调试信息
    opt = {},       -- 选项
}

-- 对外接口，是一个全局变量，调用它的函数完成操作
dbg = {
}

-- 压入命令
function dbgd.pushcmd(cmd, ...)
    dbgd.cmdlist[#dbgd.cmdlist+1] = {cmd, table.unpack{...}}
end

-- 执行命令
function dbgd.execcmd()
    local cmdlist = dbgd.cmdlist
    dbgd.cmdlist = {}
    for _, cmdinfo in ipairs(cmdlist) do
        local cmd = cmdinfo[1]
        if dbgd[cmd] then
            dbgd[cmd](table.unpack(cmdinfo, 2))
        else
            print(string.format("error - unknown cmd: %s", cmd))
        end
    end
end

-- 取函数名
function getname(n)
    if n.what == "C" then
        return n.name
    end
    local lc = string.format("%s:%d", n.short_src, n.currentline)
    if n.what ~= "main" and n.namewhat ~= "" then
        return string.format("%s (%s)", lc, n.name)
    else
        return lc
    end
end

-- 保存函数的各种信息
function dbgd.capinfo()
    local level = 4
    local finfo = debug.getinfo(level, "nSlf")
    local info = {}
    -- function info
    info.name = getname(finfo)
    info.func = finfo.func
    -- upvalues
    info.uv = {}
    local i = 1
    while true do
        local name, value = debug.getupvalue(finfo.func, i)
        if name == nil then break end
        if string.sub(name, 1, 1) ~= "(" then
            table.insert(info.uv, {name, value, i})
        end
        i = i + 1
    end
    -- local values
    info.lv = {}
    i = 1
    while true do
        local name, value = debug.getlocal(level, i)
        if not name then break end
        if string.sub(name, 1, 1) ~= "(" then
            table.insert(info.lv, {name, value, i})
        end
        i = i + 1
    end
    -- vararg arguments
    info.av = {}
    i = -1
    while true do
        local name, value = debug.getlocal(level, i)
        if not name then break end
        if string.sub(name, 1, 1) ~= "(" then
            table.insert(info.av, {name, value, i})
        end
        i = i -1
    end
    dbgd.info = info
end

-- 进入交互界面
local function interactive()
    dbgd.resume()
    print(debug.traceback(nil, 3))
    dbgd.capinfo()
    debug.debug()
    dbgd.execcmd()
end

--- 调试器Hook回调
function dbgd.hook(evt, arg)
    if evt == 'call' then
        interactive()
    elseif evt == "line" then
        if dbgd.opt.type == "line" then
            if dbgd.opt.line == arg then
                interactive()
            end
        elseif dbgd.opt.type == "stepin" then
            interactive()
        elseif dbgd.opt.type == "stepover" then
            if debug.getinfo(2, "f").func == dbgd.info.func then
                interactive()
            end
        end
    end
end

--- 在某行加一个断点
function dbgd.breakpoint(line)
    dbgd.opt.type = "line"
    dbgd.opt.line = line
    debug.sethook(dbgd.hook, "l")
end

-- 单步进入
function dbgd.stepin()
    dbgd.opt.type = "stepin"
    debug.sethook(dbgd.hook, "l")
end

-- 单步跳过
function dbgd.stepover()
    dbgd.opt.type = "stepover"
    debug.sethook(dbgd.hook, "l")
end

-- 设置upvalue
function dbgd.setupvalue(n, v)
    debug.setupvalue(dbgd.info.func, n, v)
end

-- 设置本地变量
function dbgd.setlocalvalue(n, v)
    debug.setlocal (5, n, v)
end

--- 删除Hook，继续执行
function dbgd.resume()
    debug.sethook()
end

---------------------------------------------------------

-- 对外的命令接口
-- 帮助
function dbg.h() 
    print("dbg.h()\t\t\tprint help")        -- 帮助
    print("dbg.bp(line)\t\tadd a breakpoint to a line")     -- 在第几行断点
    print("dbg.si()\t\tstep into next function call")       -- 单步执行
    print("dbg.so()\t\tstep over next function call")       -- 单步执行(跳过函数)

    print("dbg.all()\t\tprint all debug info")      -- 打印所有的信息
    print("dbg.name()\t\tprint function name")     -- 打印函数信息
    print("dbg.uv()\t\tprint up values")            -- 打印upvalue
    print("dbg.lv()\t\tprint local values")         -- 打印局部变量(包括参数)
    print("dbg.av()\t\tprint vararg arguments")       -- 打印可变参数

    print("dbg.setuv(n, v)\t\tchange a upvalue")    -- 设置upvalue，n是变量的序号
    print("dbg.setlv(n, v)\t\tchange a local value")  -- 设置局部变量，n是变量的序号
end

local function print_vars(msg, vars)
    print(msg)
    if vars then
        for _, v in ipairs(vars) do
            print("", v[3], v[1], v[2])
        end
    end
end

function dbg.name()
    print("name: ")
    print(string.format("    %s", dbgd.info.name))
end

function dbg.uv() 
    print_vars("up values: ", dbgd.info.uv)
end

function dbg.lv() 
    print_vars("local values: ", dbgd.info.lv)
end

function dbg.av() 
    print_vars("vararg argument: ", dbgd.info.av)
end

function dbg.all()
    dbg.name()
    dbg.uv()
    dbg.lv()
    dbg.av()
end

function dbg.bp(ln)
    dbgd.pushcmd("breakpoint", ln)
end

function dbg.si()
    dbgd.pushcmd("stepin")
end

function dbg.so()
    dbgd.pushcmd("stepover")
end

function dbg.setuv(n, v)
    dbgd.pushcmd("setupvalue", n, v)
end

function dbg.setlv(n, v)
    dbgd.pushcmd("setlocalvalue", n, v)
end

local function run(luacode)
    local chunk = loadfile(luacode)
    debug.sethook(dbgd.hook, "c")
    chunk()
    debug.sethook()
end


run(select(1, ...))




