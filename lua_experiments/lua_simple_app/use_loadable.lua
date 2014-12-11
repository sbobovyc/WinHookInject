-- load the c-function library
io.write("[Lua] Loading the C-function library\n")
--io.write(package.cpath)
cfunctions = require("libloadable")

-- use some of the functions defined in the library
io.write("[Lua] Executing the C-library functions\n");
local f = cfunctions.fun1()
io.write("[Lua] Function 1 says it's ", f, " times funny\n"); 
f = cfunctions.fun2()
io.write("[Lua] Function 2 says it's ", f, " times funny\n"); 
io.write("[Lua] Exiting Lua script\n")