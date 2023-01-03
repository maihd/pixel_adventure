--
-- Copyright (c) 2018 Milos Tosic. All rights reserved.
-- License: http://www.opensource.org/licenses/BSD-2-Clause
--

local currPath = path.getdirectory(debug.getinfo(2, "S").source:sub(2)) .. "/.."
while string.len(currPath) > 2 do 
	currPath = path.getabsolute(currPath .. "/..")
	if os.isfile(currPath .. "/build/build.lua") then dofile (currPath .. "/build/build.lua") break end
end

solution "rmem"
	configurations { "debug", "release", "retail" }
	setPlatforms()

	addProject_lib("rmem")
	addProject_lib_sample("rmem", "linker", false)
	addProject_lib_sample("rmem", "manual", false)

