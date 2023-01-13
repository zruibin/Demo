#! /usr/bin/env lua
--
-- main.lua
--
-- Created by Ruibin.Chow on 2022/11/09.
-- Copyright (c) 2022年 Ruibin.Chow All rights reserved.
--


-- package.path = "./libs/?/?.lua;"..package.path
-- print(package.path);


require("libs/logger/logger")


local function markdownMain()
	Logger.v("Markdown Main.")
	local markdown = require("libs/markdown/markdown")
	local source = [[
# This is an h1
## This is an h2
###### This is an h6
# This is an h1 #
## This is an h2 ##
### This is an h3 ######
<span>haha</span>
	]]
	Logger.d("Source: " .. source)
	local html = markdown(source)
	Logger.d("html: " .. html)
end


local function testLogTable()
	myData = {
	    null = nil,
	    bool = true,
	    num = 20,
	    str = "abc",
	    subTab = {"111", "222"},
	    func = print_dump,
	    sunTab = {"sun_a", {"sun_1", "sun_2"}, {you = "god", i = "man"}}
	}
	Logger.i(myData)
end






local function main()
	markdownMain()
	testLogTable()
end

main()
