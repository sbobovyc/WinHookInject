#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include <stdlib.h>
#include <string.h>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

int main(void) {
    std::cout << lua_version() << std::endl;
    lua_State *L = lua_open();
    // load the libs, Lua 5.0
    luaopen_base(L);
    luaopen_table(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);
    
    lua_dofile(L, "test.lua");
    
    lua_close(L);
    return 0;
}