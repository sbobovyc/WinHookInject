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

void print_table(lua_State *L)
{
    while(lua_next(L, -2) != 0) {
        if(lua_isstring(L, -1)) {
          std::cout << lua_tostring(L, -2) << " = " << lua_tostring(L, -1) << std::endl;
        } else if(lua_isnumber(L, -1)) {
          std::cout << lua_tostring(L, -2) << " = " << lua_tonumber(L, -1) << std::endl;
        } else if(lua_istable(L, -1)) {
            std::cout << lua_tostring(L, -2) << " = " << lua_topointer(L, -1) << std::endl;
        } else if(lua_isfunction(L, -1))
            std::cout << lua_tostring(L, -2) << " = " << lua_topointer(L, -1) << std::endl;

        lua_pop(L, 1);
    }
}

int main(void) {
    std::cout << lua_version() << std::endl;       
    lua_State *L = lua_open();
    std::cout << "This is the address of lua state: " << L << std::endl;
    // load the libs, Lua 5.0
    luaopen_base(L);
    luaopen_table(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);
    
    lua_dofile(L, "test.lua");

	FILE * fstate = fopen("luastate", "w");
	fprintf(fstate, "%i", (int)L);
	fclose(fstate);
    
    lua_getglobal(L, "_G");
    lua_pushnil(L);               // put a nil key on stack    
    print_table(L);
    
    do {
        std::cout << std::endl << "Press enter to continue...";
    } while (std::cin.get() != '\n');    
    
    lua_close(L);
    return 0;
}