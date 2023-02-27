#! /usr/bin/env lua
--
-- readonly_table.lua
--
-- Created by Ruibin.Chow on 2023/02/27.
-- Copyright (c) 2023年 Ruibin.Chow All rights reserved.
--


--[=[
https://km.woa.com/articles/show/565784
如果使用rawset(类似的还有rawget) 绕过元方法来设置table,那么 table 仍然会被更新(而不能做到只读)。
目前就需要在宿主语言侧(譬如 C)来实现只读的 table 类型,并导出给 Lua 来使用.
]=]

local proxies = {}

local function readonly(input_tbl)
    if type(input_tbl) ~= "table" then
        return input_tbl
    end

    -- 检查缓存
    local proxy = proxies[input_tbl]
    if not proxy then
      proxy = {
        __next = function(t, k) return next(t, k) end,
      }
      local mt = {
        __index = input_tbl,
        __newindex = function(t, k, v)
          error("attempt to update a readonly table, key: " .. tostring(k))
        end,
        __pairs = function(t) return pairs(input_tbl) end,
        __ipairs = function(t) return ipairs(input_tbl) end,
        __len = function(t) return #input_tbl end,
      }
      setmetatable(proxy, mt)
      proxies[input_tbl] = proxy

      -- 所有内部表都变成readonly
      for k, v in pairs(input_tbl) do
        if type(v) == "table" then
            input_tbl[k] = readonly(v)
        end
      end
    end

    return proxy
end

--------------------------------------------------------------------------------

local function testread()
    local key = {
        a = "a"
    }
    local data = {
        [3] = 3,
        [key] = "value"
    }

    local data2 = readonly(data)
    local key2 = readonly(key)

    print("read data2[3]:", data2[3])
    print("read data2[key]:", data2[key])
    print("read data2[key2]:", data2[key2])

    local data3 = {
        [key] = "key value",
        [key2] = "key2 value"
    }

    local data4 = readonly(data3)
    print("read data4[key]:", data4[key])
    print("read data4[key2]:", data4[key2])
end

local function testipairsread()
    local data = {
        [1] = 1,
        [2] = 2,
        [3] = 3,
        [4] = 4
    }

    local data2 = readonly(data)
    print("len data2:", #data2, "data:", #data)

    for i, v in ipairs(data2) do
        print(i, v)
    end
end

local function testpairsread()
    local key = {
        a = "a"
    }
    local data = {
        [key] = "key",
        [readonly(key)] = "readonly(key)",
        [4] = 4,
        ["str"] = "str"
    }

    local data2 = readonly(data)

    for i, v in pairs(data2) do
        print(i, v)
    end
end

local function testwrite1()
    local data = {
        ["a"] = 1
    }

    local data2 = readonly(data)

    data2.a = 2
end

local function testwrite2()
    local data = {
        ["a"] = {
            ["b"] = 1
        }
    }

    local data2 = readonly(data)

    data2.a.b = 2
end

local function testwrite3()
    local data = {
        ["a"] = {
            ["b"] = 1
        }
    }

    local data2 = readonly(data)

    for k, v in pairs(data2) do
        print(k, v)
        v.b = 2
    end
end

local function testwrite4()
    local key1 = {
        ["k1"] = "k1"
    }
    local key2 = {
        ["k2"] = "k2"
    }
    local data = {
        [key1] = {
            ["b"] = 1
        },
        [key2] = {
            ["b"] = 1
        }
    }

    local data2 = readonly(data)

    for k, v in pairs(data2) do
        print(k, v)
        for k1, v1 in pairs(k) do
            print(k1, v1)
        end
        for k2, v2 in pairs(v) do
            print(k2, v2)
        end
    end

    for k, v in pairs(data2) do
        k.a = 1
    end
end

local function testrawset()
    local data = {
        ["a"] = 1
    }

    local data2 = readonly(data)

    print( "before rawset data2:", data2.a)
    rawset(data2, "a", 2)
    print( "after rawset data2:", data2.a)
end

local function testinsert()
    local data = {
        [1] = 1,
        [2] = 2,
        [3] = 3,
        [4] = 4
    }

    local data2 = readonly(data)

    table.insert(data2, 5)
end

local function testremove()
    local data = {
        [1] = 1,
        [2] = 2,
        [3] = 3,
        [4] = 4
    }

    local data2 = readonly(data)

    table.remove(data2, 1)
end

local function testcompare()
    local data = {
        [1] = 1
    }

    local data2 = readonly(data)

    print("data == data2:", data == data2)
    print("data2 == data:", data2 == data)

    local data3 = {
        [1] = 1
    }

    print("data == data3:", data == data3)
    print("data3 == data:", data3 == data)

    print("data2 == data3:", data2 == data3)
    print("data3 == data2:", data3 == data2)

    local data4 = readonly(data3)

    print("data2 == data4:", data2 == data4)
    print("data4 == data2:", data4 == data2)
end

local function errorhandle(errobject)
    print(debug.traceback(errobject))
end


local function mytest()
    print("exec testread")
    xpcall(testread, errorhandle)

    print("---------------------")

    print("exec testipairsread")
    xpcall(testipairsread, errorhandle)

    print("---------------------")

    print("exec testpairsread")
    xpcall(testpairsread, errorhandle)

    print("---------------------")

    print("exec testwrite1")
    xpcall(testwrite1, errorhandle)

    print("---------------------")

    print("exec testwrite2")
    xpcall(testwrite2, errorhandle)

    print("---------------------")

    print("exec testwrite3")
    xpcall(testwrite3, errorhandle)

    print("---------------------")

    print("exec testwrite4")
    xpcall(testwrite4, errorhandle)

    print("---------------------")

    print("exec testrawset")
    xpcall(testrawset, errorhandle)

    print("---------------------")

    print("exec testinsert")
    xpcall(testinsert, errorhandle)

    print("---------------------")

    print("exec testremove")
    xpcall(testremove, errorhandle)

    print("---------------------")

    print("exec testcompare")
    xpcall(testcompare, errorhandle)
end

mytest()