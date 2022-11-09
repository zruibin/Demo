#! /usr/bin/env lua
--
-- main.lua
--
-- Created by Ruibin.Chow on 2022/11/09.
-- Copyright (c) 2022年 Ruibin.Chow All rights reserved.
--


package.path = "./libs/?/?.lua;"..package.path
print(package.path);




local function markdownMain()
	print("Markdown Main.")
	local markdown = require "markdown"
	local source = [[
# This is an h1
## This is an h2
###### This is an h6
# This is an h1 #
## This is an h2 ##
### This is an h3 ######
<span>haha</span>
	]]
	print("Source: " .. source)
	local html = markdown(source)
	print("html: " .. html)
end












local function main()
	markdownMain()
end

main()